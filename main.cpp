#include <SFML/Graphics.hpp>

#include "Geodesic.hpp"
#include "Particle.hpp"
#include "Spacetime.hpp"

#include "SkyMap.hpp"
#include "FileParser.hpp"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

struct Config{   
    std::vector<double> position; // spherical coords
    std::vector<double> velocity; // light speed proportional
    double inclination; // view inclination angle in degrees
    double azimuth; // view azimuth angle in degrees
    double fov_horizontal; // horizontal field of view
    double fov_vertical; // vertical field of view
    int dimensions; // width of the resulting image
    bool do_elliptic_parametrization;
    double grid_angle; // angle between each line of the grid in degrees, latitude/longitude
    double grid_thickness; // thickness of each grid line in degrees
};

void readConfigFile(const std::string& config_file, Config& config){
    FileParser fp (config_file, true);
    fp.get("position", config.position);
    fp.get("velocity", config.velocity);
    fp.get("inclination", config.inclination);
    fp.get("azimuth", config.azimuth);
    fp.get("fov_horizontal", config.fov_horizontal);
    fp.get("fov_vertical", config.fov_vertical);
    fp.get("dimensions", config.dimensions);
    fp.get("do_elliptic_parametrization", config.do_elliptic_parametrization);
    fp.get("grid_angle", config.grid_angle);
    fp.get("grid_thickness", config.grid_thickness);
}

double toRadians(const double degrees){
    return degrees / 180.0 * M_PI;
}

int main()
{
    Config config;
    readConfigFile("config.txt", config);

    // ############### SPACETIME ###############
    // Flat st;
    
    const double M = 0.25;
    Schwarzschild st(M);

    // double M = 0.25;
    // double a = 0.;
    // Kerr st(M, a);

    // ############### OBSERVER ###############
        
    VecD position = {config.position[0], toRadians(config.position[1]), toRadians(config.position[2])};
    const double alpha = toRadians(config.inclination);
    const double beta = toRadians(config.azimuth);
    // arguments: Spacetime, position, velocity, view_alpha, view_beta, velocity squared
    Particle par(st, position, config.velocity, alpha, beta, -1.);
    
    const double field_of_view_horizontal = toRadians(config.fov_horizontal);
    const double field_of_view_vertical = toRadians(config.fov_vertical);
    par.setAngleViews(field_of_view_horizontal, field_of_view_vertical);
    
    // create a matrix with the positional information of the pixel in the 
    // original sky for each pixel inside the view
    // matrix of size  dim x dim, if the horizontal and vertical angles 
    // of the FOV are the same
    auto mat = par.view(config.dimensions);

    // ############### SKY ###############
    
    // load an image to fill the sky
    // ImageSkyMap sky(st, "res/images/stars_3.jpg", do_elliptic_parametrization);
    
    // color the sky with 8 colors
    const double grid_angle =  toRadians(config.grid_angle);
    const double thickness = toRadians(config.grid_thickness);
    ColorSkyMap sky(st, config.do_elliptic_parametrization, grid_angle, thickness);

    // ############### RESULTS ###############
    
    sf::Image view = sky.getSkyView(mat);
    view.saveToFile("tmp/screenshot" + std::to_string(time(NULL)) + ".png");

    return 0;
}
