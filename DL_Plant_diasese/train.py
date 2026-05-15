import tensorflow as tf
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Conv2D, MaxPooling2D
from tensorflow.keras.layers import Flatten, Dense

# Load Dataset
train_data = tf.keras.preprocessing.image_dataset_from_directory(
    "dataset",
    image_size=(128, 128),
    batch_size=32
)

# Normalize Images
train_data = train_data.map(lambda x, y: (x / 255.0, y))

# Build CNN Model
model = Sequential([

    Conv2D(32, (3,3), activation='relu', input_shape=(128,128,3)),
    MaxPooling2D(2,2),

    Conv2D(64, (3,3), activation='relu'),
    MaxPooling2D(2,2),

    Flatten(),

    Dense(128, activation='relu'),

    Dense(1, activation='sigmoid')
])

# Compile Model
model.compile(
    optimizer='adam',
    loss='binary_crossentropy',
    metrics=['accuracy']
)

# Train Model
model.fit(train_data, epochs=15)

# Save Model
model.save("plant_disease_model.h5")

print("Model Trained Successfully")