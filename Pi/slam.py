import settings
import open3d as o3d
import cv2
import numpy as np

class Slam:
    """Class for Simultaneous Localization and Mapping (SLAM)."""

    def __init__(self):
        """
        Initialize the SLAM object.

        Attributes:
        - pcd (open3d.geometry.PointCloud): Point cloud data.
        - visualizer (open3d.visualization.Visualizer): Open3D visualizer object.
        - pointcloud_window_created (bool): Flag indicating if the point cloud window is created.
        - oldFrame (tuple): Tuple containing the old frame data (image, (sift, des)).
        - newFrame (tuple): Tuple containing the new frame data (image, (sift, des)).
        - Tpos (np.array): Starting position.
        - Rpos (np.array): Starting rotation.
        - movement (list): List of positions.
        """
        self.pcd = o3d.geometry.PointCloud()
        self.visualizer = o3d.visualization.Visualizer()
        self.pointcloud_window_created = False
        self.oldFrame = None
        self.newFrame = None

        self.Tpos = np.array(
            [0, 0, 0], 
            dtype=float)
        
        self.Rpos = np.array([
            [1, 0, 0],
            [0, 1, 0],
            [0, 0, 1]], 
            dtype=float)
        
        self.movement = []
    
    def imageRescale(self, img, scale):
        """
        Rescale an image.

        Args:
        - img (np.array): Image to be rescaled.
        - scale (float): Scaling factor.

        Returns:
        - resized (np.array): Rescaled image.
        """
        width = int(img.shape[1] * scale)
        height = int(img.shape[0] * scale)
        dim = (width, height)
        
        # resize image
        resized = cv2.resize(img, dim, interpolation = cv2.INTER_AREA)
        return resized

    def SIFT(self, img):
        """
        Detect SIFT keypoints in an image.

        Args:
        - img (np.array): Image for detecting keypoints.

        Returns:
        - tuple: Tuple containing keypoints and descriptors.
        """
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
        sift = cv2.SIFT_create()
        kp, des = sift.detectAndCompute(gray, None)

        return (kp, des)

    def FLANN(self, des1, des2, filter):
        """
        Perform matching of keypoints using the FLANN algorithm.

        Args:
        - des1 (np.array): Descriptors for keypoints in the first image.
        - des2 (np.array): Descriptors for keypoints in the second image.
        - filter (float): Matching filter threshold.

        Returns:
        - goodMatches (list): List of matching keypoints.
        """
        # FLANN parameters
        FLANN_INDEX_KDTREE = 1
        index_params = dict(algorithm = FLANN_INDEX_KDTREE, trees = 5)
        search_params = dict(checks = 50)   # Or pass empty dictionary

        flann = cv2.FlannBasedMatcher(index_params,search_params)
        matches = flann.knnMatch(des1, des2, k = 2)

        # Apply ratio test
        goodMatches = []
        for m,n in matches:
            if m.distance < filter * n.distance:
                goodMatches.append([m])

        return goodMatches

    def kpToNpArray(self, kp):
        """
        Convert keypoints to a NumPy array.

        Args:
        - kp: Keypoints.

        Returns:
        - np.array: NumPy array of keypoints.
        """
        return np.array([x.pt for x in kp], dtype=np.float32)

    def setSlamData(payload):
        """
        Set SLAM data.

        Args:
        - payload: Payload data.
        """
        movement, pcd = payload
        self.movement = movement
        self.pcd = pcd

    def visualizeSlamData():
        """Visualize SLAM data."""
        self.pcd.clear()
        self.pcd.points = o3d.utility.Vector3dVector(self.movement)
        self.visualizer.remove_geometry(self.pcd)
        self.visualizer.add_geometry(self.pcd)
        self.visualizer.poll_events()
        self.visualizer.update_renderer()

    def do_slam(self, frame):
        """
        Perform SLAM on a frame.

        Args:
        - frame (np.array): Input frame for SLAM.
        """
        if not self.pointcloud_window_created:
            self.visualizer.create_window(window_name="3D plot", width=settings.RESOLUTION_VGA[0], height=settings.RESOLUTION_VGA[1])
            self.pointcloud_window_created = True
        
        workinFrame = self.imageRescale(frame, settings.SLAM_IMAGE_SCALE)

        # Find and store keypoints
        keypoints = self.SIFT(workinFrame)
        self.oldFrame = self.newFrame
        self.newFrame = (workinFrame, keypoints)

        # Not enough frames to continue
        if ((self.oldFrame == None) or (self.newFrame == None)):
            return

        # Find matching keypoints 
        try:
            matches = self.FLANN(self.oldFrame[1][1], self.newFrame[1][1], filter=settings.SLAM_MATCHING_RATIO_THRESHOLD)
        except:
            print("FLANN match fail")
            return

        # Not enough keypoins to continue (minimum 5)
        if (len(matches) < 5):
            print("not enough keypoins to continue (minimum 5)")
            cv2.imshow('Frame', workinFrame) 
            return

        trackImage = workinFrame.copy()

        # Retrieve matching keypoints
        oldImgKpMatches = np.zeros((len(matches), 2))
        newImgKpMatches = np.zeros((len(matches), 2))

        for index, match in enumerate(matches):
            oldKp = self.oldFrame[1][0][match[0].queryIdx]
            newKp = self.newFrame[1][0][match[0].trainIdx]

            oldImgKpMatches[index] = oldKp.pt
            newImgKpMatches[index] = newKp.pt
            
            # Draw line 
            cv2.line(trackImage, 
                #       X-pos V           Y-Pos V
                (int(oldKp.pt[0]), int(oldKp.pt[1])),# < Previous frame
                (int(newKp.pt[0]), int(newKp.pt[1])),# < Next frame
                (0, 255, 0), 2) 
            
        cv2.imshow('Frame', trackImage)

        # Pose recovery
        try:
            E, _ = cv2.findEssentialMat(
                oldImgKpMatches,
                newImgKpMatches,
                settings.CAMERA_FOCAL_SETTING,
                settings.CAMERA_PRINCIPAL_POINT_SETTING,
                cv2.RANSAC,
                0.999,
                1.0,
                None)

            _, R, t, mask = cv2.recoverPose(E, oldImgKpMatches, newImgKpMatches)
        except:
            print("Pose recovery fail")
            return

        # Position update
        self.Tpos = self.Tpos + self.Rpos.dot(t)
        self.Rpos = R.dot(self.Rpos)

        newPoint = self.Tpos[:,0]
        self.movement.append(newPoint)

        #print(newPoint)

        # Update SLAM window
        self.visualizeSlamData()