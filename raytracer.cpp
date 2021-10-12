#include <fstream>
#include <cstdio>
#include <sys/stat.h>
#include <vector>

#include "headers/Commons.h"
#include "headers/Color.h"
#include "headers/Ray.h"
#include "headers/Vec3.h"
#include "headers/HittableList.h"
#include "headers/Sphere.h"
#include "headers/Camera.h"

using namespace std;

inline bool file_exists(const string &name)
{
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

inline void save_file(vector<Color> pixelValues, const int width, const int height, const int samples_per_pixel)
{
    char fileName[] = "raytrace.ppm";
    // Delete old image if it exists
    if (file_exists(fileName))
    {
        remove(fileName);
    }

    ofstream ofs("raytrace.ppm", ios_base::out | ios_base::binary);
    ofs << "P3" << endl
        << width << ' ' << height << endl
        << "255" << endl;

    for (auto const &pixel : pixelValues)
    {
        write_color(ofs, pixel, samples_per_pixel);
    }

    ofs.close();
    cerr << "\nDone.\n";
}

Color ray_color(const Ray &r, const HittableList &world, int depth)
{
    HitRecord rec;

    if (depth <= 0)
        return Color(0, 0, 0);

    if (world.hit(r, 0.002, infinity, rec))
    {
        Point3 target = rec.p + rec.normal + ranomd_in_hemisphere(rec.normal);
        return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
    }
    Vec3 unit_direction = unit_vector(r.direction());
    double t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * Color(1, 1, 1) + t * Color(0.5, 0.7, 1.0);
}

vector<Color> generate_image(
    const double aspect_ratio,
    const int width,
    const int height,
    const HittableList &world,
    const int samples_per_pixel,
    const int max_depth)
{
    // Empty Vector
    vector<Color> p;

    // Camera
    Camera cam(aspect_ratio);

    // Generate Pixels
    for (int j = height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < width; ++i)
        {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                double u = (i + random_double()) / (width - 1);
                double v = (j + random_double()) / (height - 1);
                Ray r = cam.getRay(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            p.push_back(pixel_color);
        }
    }
    cerr << "\nDone.\n";

    return p;
}

int main()
{
    // Screen
    const double aspect_ratio = 16.0 / 9.0;
    const int width = 800;
    const int height = static_cast<int>(width / aspect_ratio);
    const int samples_per_pixel = 90;
    const int max_depth = 40;
    cout << "Configuration: \nWidth: " << width << "\nHeight: " << height << "\n";

    // World Setup
    HittableList world;
    world.add(make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    vector<Color> pixels = generate_image(aspect_ratio, width, height, world, samples_per_pixel, max_depth);
    save_file(pixels, width, height, samples_per_pixel);

    return EXIT_SUCCESS;
}
