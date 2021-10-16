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
#include "headers/Material.h"
#include "headers/AARect.h"

using namespace std;

//Initialize Scenes
HittableList first_default();
HittableList light_and_sphere();
HittableList cornell_box();

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

Color ray_color(const Ray &r, const Color &background, const HittableList &world, int depth)
{
    HitRecord rec;

    if (depth <= 0)
        return Color(0, 0, 0);

    if (!world.hit(r, 0.001, infinity, rec))
        return background;

    Ray scattered;
    Color attenuation;
    Color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

vector<Color> generate_image(
    const double aspect_ratio,
    const int width,
    const int height,
    const HittableList &world,
    const int samples_per_pixel,
    const int max_depth,
    const Color &background)
{
    // Empty Vector
    vector<Color> p;

    // Camera
    Camera cam(100, aspect_ratio);

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
                pixel_color += ray_color(r, background, world, max_depth);
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
    const double aspect_ratio = 4.0 / 3.0;
    const int width = 100;
    const int height = static_cast<int>(width / aspect_ratio);
    const int samples_per_pixel = 80;
    const int max_depth = 10;
    cout << "Configuration: \nWidth: " << width << "\nHeight: " << height << "\n";

    const Color background(0, 0, 0);

    // World Setup
    HittableList world = cornell_box();

    vector<Color> pixels = generate_image(aspect_ratio, width, height, world, samples_per_pixel, max_depth, background);
    save_file(pixels, width, height, samples_per_pixel);

    return EXIT_SUCCESS;
}

//Different Scenes:
HittableList first_default()
{
    HittableList world;

    shared_ptr<Lambertian> groundMaterial = make_shared<Lambertian>(Color(0.4, 0.8, 0.8));
    shared_ptr<Lambertian> lambertianMaterial = make_shared<Lambertian>(Color(0.2, 0, 0.5));
    shared_ptr<Metal> metalMaterial = make_shared<Metal>(Color(1, 0.3, 0.3), 0.4);
    shared_ptr<Dielectric> glass = make_shared<Dielectric>(1.3);

    world.add(make_shared<Sphere>(Point3(1.5, 0, -2), 0.5, metalMaterial));
    world.add(make_shared<Sphere>(Point3(0, 0, -2), 0.5, lambertianMaterial));
    world.add(make_shared<Sphere>(Point3(-1.5, 0, -2), 0.5, glass));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, groundMaterial));

    return world;
}

HittableList light_and_sphere()
{
    HittableList world;

    shared_ptr<Lambertian> sphereMat = make_shared<Lambertian>(Color(0.2, 0, 0.5));
    shared_ptr<Lambertian> groundMat = make_shared<Lambertian>(Color(0.4, 0.8, 0.8));

    shared_ptr<DiffuseLight> light = make_shared<DiffuseLight>(Color(4, 4, 4));

    world.add(make_shared<Sphere>(Point3(1.5, 0, -2), 0.5, sphereMat));
    world.add(make_shared<Sphere>(Point3(0, 0, -2), 0.5, light));
    world.add(make_shared<Sphere>(Point3(-1.5, 0, -2), 0.5, sphereMat));
    world.add(make_shared<Sphere>(Point3(0, -100.5, -1), 100, groundMat));

    return world;
}

HittableList cornell_box()
{
    HittableList world;

    double x, y, z;

    x = 4;
    y = 4;
    z = 6;

    // Room Materials
    shared_ptr<DiffuseLight> light = make_shared<DiffuseLight>(Color(1, 1, 1));
    shared_ptr<Lambertian> normal = make_shared<Lambertian>(Color(0.9, 0.9, 0.9));
    shared_ptr<Lambertian> left = make_shared<Lambertian>(Color(0.6, 0.2, 0.1));
    shared_ptr<Lambertian> right = make_shared<Lambertian>(Color(0.2, 0.6, 0.1));

    // Object Materials
    shared_ptr<Metal> obj1Mat = make_shared<Metal>(Color(0.3, 0.2, 0.5), 0.5);
    shared_ptr<Metal> obj2Mat = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0);
    shared_ptr<Dielectric> glass = make_shared<Dielectric>(1.0);

    // Box
    world.add(make_shared<XYRect>(-x, x, -y, y, -z, normal)); // Front
    world.add(make_shared<XZRect>(-x, x, -z, z, -y, normal)); // Down
    world.add(make_shared<XZRect>(-x, x, -z, z, y, normal));  // Up
    world.add(make_shared<YZRect>(-y, y, -z, z, -x, left));   // Left
    world.add(make_shared<YZRect>(-y, y, -z, z, x, right));   // Right
    world.add(make_shared<XYRect>(-x, x, -y, y, z, normal));  // Back - Enclose light

    // Light source
    world.add(make_shared<XZRect>(-2, 2, -z + 1, z - 1, y - 1e-8, light));

    // Other objects

    world.add(make_shared<Sphere>(Point3(-1.5, -y + 1.2, -3.5), 1.2, obj1Mat)); //Left
    world.add(make_shared<Sphere>(Point3(1, -2.1, -3), 0.7, obj2Mat));

    world.add(make_shared<XYRect>(-x, x, -y, y, -2, glass)); // Glass wall

    return world;
}