from pyzbar import pyzbar
import cv2
import numpy as np

image = cv2.imread("C:\\Users\\Nick\\Pictures\\qr8.png")

# add some noise and bad stuff to the image for testing

a = np.array(image, dtype="int32")
b = a * 0.9
# np.clip(b, 0, 255)
image2 = np.array(b, dtype="uint8")
noise = np.zeros(image.shape, np.uint8)
cv2.randn(noise, 0, 64)
image3 = cv2.add(image2, noise)

r, c, ch = image3.shape

pts1 = np.array([
    [0, r],
    [c, r],
    [0, 0],
    [c, 0]
], dtype="float32")

pts2 = np.array([
    [0, r],
    [c, r * 0.7],
    [0, 0],
    [c, 0.1 * r]
], dtype="float32")

tf = cv2.getPerspectiveTransform(pts1, pts2)
image4 = cv2.warpPerspective(image3, tf, (c, r))

# find qr code looking things
fov = (90, 60) # field of view horizontal, vertical
res = (640, 480) # width, height
target = (5, 5) # width, height of qr code in inches

barcodes = pyzbar.decode(image)

detector = cv2.QRCodeDetector()

for bc in barcodes:
    print("")
    print(bc.rect)
    print(bc.data.decode("utf-8"))

    x, y, w, h = bc.rect
    xadj = int(x - w / 10)
    wadj = int(w * 1.2)
    yadj = int(y - h / 10)
    hadj = int(h * 1.2)
    cropped = image[yadj:yadj + hadj, xadj:xadj + wadj]
    # cropped = image[y:y+h, x:x+w]
    gray = cv2.cvtColor(cropped, cv2.COLOR_BGR2GRAY)
    # blur = cv2.GaussianBlur(gray, (3, 3), 0)
    # _, thresh = cv2.threshold(gray, 150, 128, 0, 0)
    _, thresh = cv2.threshold(gray, 0, 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
    _, rect = detector.detect(gray)
    print(rect)
    if not (rect is None) and len(rect) > 0:
        rect = np.array(rect, dtype="int32")
        cv2.polylines(cropped, rect, True, [255, 0, 0], 2)
    cv2.imshow("stuff", gray)
    cv2.waitKey()

# try to get relative position
    dist = target[1] * res[1] / (bc.rect.height * np.sin(fov[1]))

cv2.imshow("barcodes", image)
cv2.waitKey()