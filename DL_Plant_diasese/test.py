import tensorflow as tf
import numpy as np
from tensorflow.keras.preprocessing import image

# Load Model
model = tf.keras.models.load_model("plant_disease_model.h5")

# Load Test Image
img = image.load_img(
    r"C:\Users\HP\Documents\Diff_Projects\DL\dataset\Diseased\Potato___Early_blight2.JPG",
    target_size=(128,128)
)

# Convert Image to Array
img_array = image.img_to_array(img)

# Add Extra Dimension
img_array = np.expand_dims(img_array, axis=0)

# Normalize
img_array = img_array / 255.0

# Prediction
prediction = model.predict(img_array)

print("Prediction Value:", prediction[0][0])

# Final Result
if prediction[0][0] > 0.5:
    print("Healthy")
else:
    print("Diseased")