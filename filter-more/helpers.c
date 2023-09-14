#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Steps through all pixel going from top left to bot right
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {

            int red = image[y][x].rgbtRed;
            int green = image[y][x].rgbtGreen;
            int blue = image[y][x].rgbtBlue;

            // Takes avg of RGB values to get correct darkness/brightness value
            int avg = round(( red + green + blue) / 3.0);

            // Set all equal RGB values equal to avg
            image[y][x].rgbtRed = avg;
            image[y][x].rgbtGreen = avg;
            image[y][x].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    // Steps through all pixel going from top left to bot right
    for (int y = 0; y < height; y++)
    {
        // Swaps left most of a row with right most until it gets to the middle of a row
        for (int x = 0; x < width / 2; x++)
        {
            int tmpred = image[y][x].rgbtRed;
            int tmpgreen = image[y][x].rgbtGreen;
            int tmpblue = image[y][x].rgbtBlue;

            image[y][x].rgbtRed = image[y][width - (x + 1)].rgbtRed;
            image[y][x].rgbtGreen = image[y][width - (x + 1)].rgbtGreen;
            image[y][x].rgbtBlue = image[y][width - (x + 1)].rgbtBlue;

            image[y][width - (x + 1)].rgbtRed = tmpred;
            image[y][width - (x + 1)].rgbtGreen = tmpgreen;
            image[y][width - (x + 1)].rgbtBlue = tmpblue;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Duplicate
    RGBTRIPLE tmp[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            tmp[y][x] = image[y][x];
        }
    }

    // Steps through all pixel going from top left to bot right
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float avgRed = 0;
            float avgGreen = 0;
            float avgBlue = 0;
            int count = 0;

            // Change this value to change the intensity of blur, larger = more blur
            int intensity = 1;

            for (int row = -intensity; row < (intensity + 1); row++)
            {
                for (int col = -intensity; col < (intensity + 1); col++)
                {

                    // Only Adds values of index, if in rows & cols
                    if ((y + row) >= 0 && (y + row) < (height) && (x + col) >= 0 && (x + col) < (width))
                    {
                        // Adds the values of each color in the 3x3 around the pixel
                        avgRed = avgRed + tmp[y + row][x + col].rgbtRed;
                        avgGreen = avgGreen + tmp[y + row][x + col].rgbtGreen;
                        avgBlue = avgBlue + tmp[y + row][x + col].rgbtBlue;
                        count++;
                    }

                }

            }

            // Takes avg of RGB values to get correct blur value, How to
            avgRed = round(((avgRed) / count));
            avgGreen = round(((avgGreen) / count));
            avgBlue = round(((avgBlue) / count));

            // Set each RGB values equal to avg
            image[y][x].rgbtRed = avgRed;
            image[y][x].rgbtGreen = avgGreen;
            image[y][x].rgbtBlue = avgBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // Duplicate
    RGBTRIPLE tmp[height][width];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            tmp[y][x] = image[y][x];
        }
    }

    // Steps through all pixel going from top left to bot right
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
            int gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
            float gxRed = 0;
            float gxGreen = 0;
            float gxBlue = 0;
            float gyRed = 0;
            float gyGreen = 0;
            float gyBlue = 0;
            float red = 0;
            float green = 0;
            float blue = 0;

            int count = 0;

            // Change this value to change the intensity of blur, larger = more blur
            int intensity = 1;

            for (int row = -intensity; row < intensity + 1; row++)
            {
                for (int col = -intensity; col < intensity + 1; col++)
                {
                    // Won't get RGB values and add to avg if index [][] is not in image
                    if ((y + row) >= 0 && (y + row) < (height) && (x + col) >= 0 && (x + col) < (width))
                    {
                        // Adds the values of each color in the 3x3 around the pixel
                        gxRed = gxRed + (tmp[y + row][x + col].rgbtRed * gx[count]);
                        gxGreen = gxGreen + (tmp[y + row][x + col].rgbtGreen * gx[count]);
                        gxBlue = gxBlue + (tmp[y + row][x + col].rgbtBlue * gx[count]);

                        gyRed = gyRed + (tmp[y + row][x + col].rgbtRed * gy[count]);
                        gyGreen = gyGreen + (tmp[y + row][x + col].rgbtGreen * gy[count]);
                        gyBlue = gyBlue + (tmp[y + row][x + col].rgbtBlue * gy[count]);
                        count++;
                    }
                    else
                    {
                        count++;
                    }
                }
            }

            // Takes avg of RGB values to get correct blur value, How to
            red = round(sqrt((pow(gxRed, 2)) + (pow(gyRed, 2))));
            green = round(sqrt((pow(gxGreen, 2)) + (pow(gyGreen, 2))));
            blue = round(sqrt((pow(gxBlue, 2)) + (pow(gyBlue, 2))));

            if (red > 255)
            {
                red = 255;
            }

            if (green > 255)
            {
                green = 255;
            }

            if (blue > 255)
            {
                blue = 255;
            }

            // Set each RGB values equal to avg
            image[y][x].rgbtRed = red;
            image[y][x].rgbtGreen = green;
            image[y][x].rgbtBlue = blue;
        }
    }
    return;
}
