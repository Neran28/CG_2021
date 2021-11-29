#include <GLApp.h>

class MyGLApp : public GLApp
{
public:
	Image image{ 640,480 };

	Vec3 convertPosFromHSVToRGB(float x, float y)
	{
		// Make sure our arguments stay in-range
		const float h = float((int(x*360)) % 360) / 60;
		const float s = std::max(0.0f, std::min(1.0f, y));
		const float v = std::max(0.0f, std::min(1.0f, 1.0f)); // in this example v can be a constant of 1

		if (s == 0) return { v,v,v }; // Achromatic (grey)

		const int i = int(floor(h));
		const float f = h - i;
		const float p = v * (1 - s);
		const float q = v * (1 - s * f);
		const float t = v * (1 - s * (1 - f));

		switch (i)
		{
		case 0: return { v,t,p };
		case 1: return { q,v,p };
		case 2: return { p,v,t };
		case 3: return { p,q,v };
		case 4: return { t,p,v };
		default: return { v,p,q };
		}
	}


	virtual void init()
	{
		glEnv.setTitle("Color Picker");

		for (uint32_t y = 0; y < image.height; ++y)
		{
			for (uint32_t x = 0; x < image.width; ++x)
			{
				const Vec3 rgb = convertPosFromHSVToRGB(float(x) / image.width, float(y) / image.height);
				image.setNormalizedValue(x, y, 0, rgb.x());
				image.setNormalizedValue(x, y, 1, rgb.y());
				image.setNormalizedValue(x, y, 2, rgb.z());
				image.setValue(x, y, 3, 255);
			}
		}
	}

	virtual void mouseMove(double xPosition, double yPosition)
	{
		Dimensions s = glEnv.getWindowSize();
		if (xPosition < 0 || xPosition > s.width || yPosition < 0 || yPosition > s.height) return;

		const Vec3 hsv{ float(360 * xPosition / s.width),float(1.0 - yPosition / s.height),1.0f };
		const Vec3 rgb = convertPosFromHSVToRGB(float(xPosition / s.width), float(1.0 - yPosition / s.height));
		std::cout << "HSV: " << hsv << " --> RGB: " << rgb << std::endl;
	}

	virtual void draw()
	{
		drawImage(image);
	}

} myApp;

int main(int argc, char** argv)
{
	myApp.run();
	return EXIT_SUCCESS;
}
