# Modeling program
The program can make localization experiments using different methods and include time and signal speed errors.
It was created for the opportunity to choose the best localization method for a user.

## Input arguments:
* `input` (short name: `i`, required) - path to JSON file with experiment description;
* `output` (short name: `o`, required) - output directory path;
* `help` (short name: `h`, default: `false`) - return help message.

## Experiment description format:
Experiment describes in JSON format, in which use the next types:

### Point
Position of something. In a plane case contains scene:
* `x` (_double_) - first coordinate of a point;
* `y` (_double_) - second coordinate of a point.


In a spherical case:
* `latitude` (_double_) - first coordinate of a point;
* `longitude` (_double_) - second coordinate of a point.

### Restrictions
Bound of a area. Contains scene:
* `min` (_Point_) - minimal values of coordinates in a scene;
* `max` (_Point_) - maximal values of coordinates in a scene;

### Scene
Describes a scene for experiment. Contains scene:
* `restrictions` (_Restrictions_) - bound of a area under consideration;
* `c` (_double_) - signal propagation speed;
* `detectors` (_[Points]_) - array of detectors position.

In a spherical case it also includes:
* `radius` (_double_) - radius of the sphere;
* `projection` (_string_) - describe projection for map. Possible values: `Sinusoidal`, `Equirectangular`.

### Grid
Contains scene:
* `width` (_int_) - width a grid;
* `height` (_int_) - height a grid.

### ErrorGenerator
Describes random error generator. 
In linear distribution case contains scene:
* `min_value` (_double_) - minimum value of the error;
* `max_value` (_double_) - maximum value of the error;

In normal distribution case contains scene:
* `mean` (_double_) - mean of normal distribution;
* `standard_deviation` (_double_) - standard deviation of normal distribution.

### Algorithm
Describes algorithm of localization. Contains scene:
* `name` (_string_) - name the algorithm, possible values: `Direct`, `QP`, `Quadrangle`, `ElderMead`.

If you use `ElderMead` you must add scene:
* `combiner` (_string_) - name of the result combiner, possible values: `Mean`, `FilteredMean`, `Median`, `Triangle`, `TimeSum`;

If you use `ElderMead` you may add scene:
* `algorithm` (_Algorithm_) - algorithm for generation start point. If the field is empty, start point always equals (0, 0);
* `number_iteration` (_int_) - maximum step number;
* `step` (_double_) - step for generation neighborhood points from the start point;
* `alpha` (_double_) - [Elder-Mead](https://en.wikipedia.org/wiki/Nelder–Mead_method) method parameter;
* `gamma` (_double_) - [Elder-Mead](https://en.wikipedia.org/wiki/Nelder–Mead_method) method parameter;
* `rho` (_double_) - [Elder-Mead](https://en.wikipedia.org/wiki/Nelder–Mead_method) method parameter;
* `sigma` (_double_) - [Elder-Mead](https://en.wikipedia.org/wiki/Nelder–Mead_method) method parameter;

### Experiment description
Describe an experiment. Path to file with this JSON object must be specified on program start. Contains scene:
* `scene` (_Scene_) - scene;
* `number_attempts` (_unsigned int_) - number attempts of localization each lightning;
* `signal` (_Point_) - position source signal;  
* `grid` (_Grid_) - grid for errors fields;
* `c_error_generator` (_ErrorGenerator_) - generator errors in wave propagation speed;
* `time_error_generator` (_ErrorGenerator_) - generator errors in time detection;
* `algorithm` (_AlgorithmType_) - localization algorithm type;

## Output
The program generate reports in out put directory.
Output report for each scene may contain files:
* `Detectors.dat` - description position of detectors according to [format requirement](http://grapherhelp.goldensoftware.com/WTOPICS/TOP_ASCIIFILES.htm);
* `Distance.grd` - description errors in distance in signal localization on the scene according to [format requirement](http://grapherhelp.goldensoftware.com/subsys/ascii_grid_file_format.htm);
* `Time.grd` - description errors in time in signal localization on the scene according to [format requirement](http://grapherhelp.goldensoftware.com/subsys/ascii_grid_file_format.htm);
* `LocalizedSignals.dat` - description attempts of localization the signal according to [format requirement](http://grapherhelp.goldensoftware.com/WTOPICS/TOP_ASCIIFILES.htm).

All formats were chosen for possibility to visualisation data in [Surfer®](https://www.goldensoftware.com/products/surfer).
