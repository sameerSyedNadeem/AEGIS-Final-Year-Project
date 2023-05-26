# From Python
# It requires OpenCV installed for Python
import sys
import cv2
import os
from sys import platform
import argparse
import socket
import numpy as np

dir_path = os.path.dirname(os.path.realpath(__file__))
try:
    if platform == "win32":
        # Change these variables to point to the correct folder (Release/x64 etc.)
        sys.path.append(dir_path + '/../../python/openpose/Release');
        os.environ['PATH']  = os.environ['PATH'] + ';' + dir_path + '/../../x64/Release;' +  dir_path + '/../../bin;'
        import pyopenpose as op
except ImportError as e:
    print('Error: OpenPose library could not be found. Did you enable `BUILD_PYTHON` in CMake and have this Python script in the right folder?')
    raise e



# Custom Params (refer to include/openpose/flags.hpp for more parameters)
params = dict()
params["model_folder"] = "../../../models/"
params["net_resolution"] = "-1x80"
params["model_pose"] = "COCO"
params["number_people_max"] = 1
params["keypoint_scale"] = 4
#params["disable_blending"] = True

#img_file = "C:/Users/ak02888/Documents/openpose/examples/media/COCO_val2014_000000000192.jpg"

# Starting OpenPose
opWrapper = op.WrapperPython()
opWrapper.configure(params)
opWrapper.start()

#Opening OpenCV stream
stream = cv2.VideoCapture(0)


host,port = "127.0.0.1",25001
sock = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
print("Socket Made")

try:
    sock.connect((host,port))
    print("Socket Connection Made")
    while True:
        ret,img = stream.read()
        # Process Image
        datum = op.Datum()
        datum.cvInputData = img
        opWrapper.emplaceAndPop([datum])

        # Display Image
        #print(type(datum.poseKeypoints[0][0]))
        #print("Body keypoints: \n" + str(datum.poseKeypoints))
        #print(datum.poseKeypoints)
        #if datum.poseKeypoints != 2.0:
        data = np.array2string(datum.poseKeypoints[0][4], precision=2, separator=',',suppress_small=True)[1:-1]
        print(data)
        sock.sendall(data.encode("utf-8"))
        data = sock.recv(1024).decode("utf-8")
        #else:
            #print("No Human Detected")

        cv2.imshow("OpenPose 1.5.1 - Tutorial Python API", datum.cvOutputData)
        key = cv2.waitKey(1)

        if key==ord('q'):
            break
    stream.release()
    cv2.destroyAllWindows()

finally:
    sock.close()

