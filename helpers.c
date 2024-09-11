#include <math.h>

#include "helpers.h"

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Store rgb totals and the number of adjacent pixels
            float red_total = 0;
            float green_total = 0;
            float blue_total = 0;
            float adjacent_pixels = 0;

            // Check if adjacent pixels are valid
            for (int row = -1; row <= 1; row++)
            {
                for (int col = -1; col <= 1; col++)
                {
                    // Store current row and col
                    int current_row = row + i;
                    int current_col = col + j;

                    // Ensure adjacent pixel is valid by being within image
                    if (current_row >= 0 && current_row < (height) && current_col >= 0 &&
                        current_col < (width))
                    {
                        // Add to rgb totals
                        red_total += copy[current_row][current_col].rgbtRed;
                        green_total += copy[current_row][current_col].rgbtGreen;
                        blue_total += copy[current_row][current_col].rgbtBlue;

                        // Increment counter
                        adjacent_pixels++;
                    }
                }
            }

            // Calculate average rgb totals and set new rgb values for image
            image[i][j].rgbtRed = round(red_total / adjacent_pixels);
            image[i][j].rgbtGreen = round(green_total / adjacent_pixels);
            image[i][j].rgbtBlue = round(blue_total / adjacent_pixels);
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Initialize Gx and Gy Matrix
    int GxMatrix[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    int GyMatrix[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // Create copy of image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Store Gx and Gy of each rgb value
            float GxRed = 0, GxGreen = 0, GxBlue = 0;
            float GyRed = 0, GyGreen = 0, GyBlue = 0;

            // Loop through all adjacent pixels
            for (int row = -1; row <= 1; row++)
            {
                for (int col = -1; col <= 1; col++)
                {
                    // Store current row and col
                    int current_row = row + i;
                    int current_col = col + j;

                    // Check if pixel is inside of image
                    if (current_row >= 0 && current_row < (height) && current_col >= 0 &&
                        current_col < (width))
                    {
                        // Multiply rgb values of adjacent pixel by GxMatrix or GyMatrix of the
                        // corresponding location and add to Gx or Gy of corresponding color
                        GxRed +=
                            copy[current_row][current_col].rgbtRed * GxMatrix[row + 1][col + 1];
                        GxGreen +=
                            copy[current_row][current_col].rgbtGreen * GxMatrix[row + 1][col + 1];
                        GxBlue +=
                            copy[current_row][current_col].rgbtBlue * GxMatrix[row + 1][col + 1];
                        GyRed +=
                            copy[current_row][current_col].rgbtRed * GyMatrix[row + 1][col + 1];
                        GyGreen +=
                            copy[current_row][current_col].rgbtGreen * GyMatrix[row + 1][col + 1];
                        GyBlue +=
                            copy[current_row][current_col].rgbtBlue * GyMatrix[row + 1][col + 1];
                    }
                }
            }

            // Calculate Sobel operator for every rgb value of an individual pixel
            int red_sobel_operator = round(sqrt(GxRed * GxRed + GyRed * GyRed));
            int green_sobel_operator = round(sqrt(GxGreen * GxGreen + GyGreen * GyGreen));
            int blue_sobel_operator = round(sqrt(GxBlue * GxBlue + GyBlue * GyBlue));

            // Cap Sobel operator to 255
            if (red_sobel_operator > 255)
            {
                red_sobel_operator = 255;
            }
            if (green_sobel_operator > 255)
            {
                green_sobel_operator = 255;
            }
            if (blue_sobel_operator > 255)
            {
                blue_sobel_operator = 255;
            }

            // Set rgb values of image
            image[i][j].rgbtRed = red_sobel_operator;
            image[i][j].rgbtGreen = green_sobel_operator;
            image[i][j].rgbtBlue = blue_sobel_operator;
        }
    }
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take average of red, green, and blue pixel values
            int average =
                round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);

            // Update pixel values
            image[i][j].rgbtRed = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Only reflects pixels when j is less than the width divided by 2
            if (j < width / 2)
            {
                // Reflects red rgb values
                int tmpred = image[i][j].rgbtRed;
                image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
                image[i][width - j - 1].rgbtRed = tmpred;

                // Reflects green rgb values
                int tmpgreen = image[i][j].rgbtGreen;
                image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
                image[i][width - j - 1].rgbtGreen = tmpgreen;

                // Reflects blue rgb values
                int tmpblue = image[i][j].rgbtBlue;
                image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
                image[i][width - j - 1].rgbtBlue = tmpblue;
            }
        }
    }
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Loop over all pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Store original rgb values
            int original_red = image[i][j].rgbtRed;
            int original_green = image[i][j].rgbtGreen;
            int original_blue = image[i][j].rgbtBlue;

            // Compute sepia values for each rgb value
            float sepia_red =
                round(.393 * original_red + .769 * original_green + .189 * original_blue);
            float sepia_green =
                round(.349 * original_red + .686 * original_green + .168 * original_blue);
            float sepia_blue =
                round(.272 * original_red + .534 * original_green + .131 * original_blue);

            // If rgb value is greater than 255 sets value to 255, otherwise set value to sepia rgb
            // value
            if (sepia_red > 255)
            {
                image[i][j].rgbtRed = 255;
            }

            else
            {
                image[i][j].rgbtRed = sepia_red;
            }

            if (sepia_green > 255)
            {
                image[i][j].rgbtGreen = 255;
            }

            else
            {
                image[i][j].rgbtGreen = sepia_green;
            }

            if (sepia_blue > 255)
            {
                image[i][j].rgbtBlue = 255;
            }

            else
            {
                image[i][j].rgbtBlue = sepia_blue;
            }
        }
    }
}
