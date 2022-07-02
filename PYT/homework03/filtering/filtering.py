import numpy as np

# get array of pixels surrounding the pixel (pixel is included)
# array has the same shape as kernel
def get_pixel_surrounding(pixel, image, kernel_size):
    x, y = pixel
    tmp = kernel_size // 2
    out = []

    # slice_x = slice( max(x - tmp, 0), min(x + tmp + 1, image.shape[0]), None )
    # slice_y = slice( max(y - tmp, 0), min(y + tmp + 1, image.shape[1]), None )
    for i in range(-tmp, tmp + 1):
        new_x = x + i

        for j in range(-tmp, tmp + 1):
            new_y = y + j

            if new_x < 0 or new_x >= image.shape[0] or new_y < 0 or new_y >= image.shape[1]:
                value = 0
            else:
                value = image[new_x, new_y]

            out.append(value)

    return np.array(out).reshape(kernel_size, kernel_size)


# apply kernel to each pixel of the pixel surrounding and return their sum
def calculate_pixel_value(pixel, image, kernel):
    area = get_pixel_surrounding(pixel, image, kernel.shape[0])
    total = np.sum(area * kernel)
    total = min(total, 255)
    total = max(total, 0)
    return total


# to each pixel of the image apply the kernel and return changed image
def apply_kernel_to_channel(image, kernel):
    out = []

    for x in range(image.shape[0]):
         for y in range(image.shape[1]):
            value = calculate_pixel_value((x,y), image, kernel)
            out.append(value)

    return np.array(out).reshape(image.shape)


def apply_filter(image: np.array, kernel: np.array) -> np.array:
    # A given image has to have either 2 (grayscale) or 3 (RGB) dimensions
    assert image.ndim in [2, 3]
    # A given kernel has to be 2 dimensional and square
    assert kernel.ndim == 2
    assert kernel.shape[0] == kernel.shape[1]

    image = image.astype(np.float)
    kernel = np.flip(kernel)

    if image.ndim == 2:
        return apply_kernel_to_channel(image, kernel).astype(np.uint8)
    else:
        r = apply_kernel_to_channel(image[:,:,0], kernel)
        g = apply_kernel_to_channel(image[:,:,1], kernel)
        b = apply_kernel_to_channel(image[:,:,2], kernel)

        return np.dstack((r,g,b)).astype(np.uint8)
