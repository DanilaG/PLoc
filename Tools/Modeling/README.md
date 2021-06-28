# How to use the program?
You can run this program from a terminal with the next input arguments.
## Input arguments:
* `experiment` (short name: `e`, required) - path to JSON file with experiment description;
* `out` (short name: `o`, default: `""`) - output directory path;
* `time` (short name: `t`, default: `false`) - flag for analysis time errors in an experiment;
* `distance` (short name: `d`, default: `false`) - flag for analysis distance errors in an experiment;
* `text_report` (short name: `r`, default: `false`) - flag for creation text description of an experiment;
* `help` (short name: `h`, default: `false`) - return help message.

Arguments `experiment` and `out` have an implicit setting if placed they value at a first and second position of arguments.
## Example start commands:  
Next command conducts an experiment described in `"expirement.json"` file, save result, describing distance errors, and text report in `"out"` directory.  
`PLoc_Modeling --experiment="expirement.json" --out="out" --distance --text_report`  
It same with:  
`PLoc_Modeling "expirement.json" "out" -d -r`  
or:  
`PLoc_Modeling "expirement.json" "out" -dr`

## Experiment description format:
Experiments described in JSON format. In description use the next types:

### Point
Position of something. Contains scene:
* `x` (_double_) - first coordinate of a point;
* `y` (_double_) - second coordinate of a point.

### Bound
Bound of a area. Contains scene:
* `min` (_Point_) - minimal values of coordinates in a scene;
* `max` (_Point_) - maximal values of coordinates in a scene;

### Scene
Describes a scene for experiment. Contains scene:
* `bound` (_Bound_) - bound of a area under consideration;
* `c` (_double_) - signal propagation speed;
* `detectors` (_[Points]_) - array of detectors position.

### Grid
Contains scene:
* `width` (_int_) - width a grid;
* `height` (_int_) - height a grid.

### ErrorGenerator
Describes random error generator. Contains scene:
* `type` - type of generator. Can take the following values:
    * `Linear` - for generator with linear distribution;
    * `Normal` - for generator with normal distribution;
* `relative_errors` - true, if errors depend on value. In other words, if it is true value with error equal _value * (1 + error_), else _value + error_;
* Fields describing the generator.
    
#### LinearErrorGenerator scene
* `min_value` (_double_) - low generator border;
* `max_value` (_double_) - upper generator border.

#### NormalErrorGenerator scene
* `mean` (_double_) - mean of normal distribution;
* `standard_deviation` (_double_) - standard deviation of normal distribution.

### AlgorithmType
Describes localization algorithm type. Can contain next values: `"Direct"`, `"Quadrangle"`, `"Vector"`.

### CombinerType
Describes algorithm of combiner results. Can contain next values: `"Mean"`.

### Experiment description
Describe an experiment. Path to file with this JSON object must be specified on program start. Contains scene:
* `scene` (_[Scene / String]_) - array of objects describing scene for an experiment. Can contain Scene or string dates. In string must be specified a path to file with scene structure. 
* `grid` (_Grid_) - sampling grid of scene;
* `number_attempts` (_unsigned int_) - number attempts of localization in each node of the grid;
* `c_error_generator` (_ErrorGenerator_) - generator errors in wave propagation speed;
* `time_error_generator` (_ErrorGenerator_) - generator errors in time detection;
* `algorithm` (_AlgorithmType_) - localization algorithm type;
* `combiner` (_CombinerType_) - combiner results algorithm type.

## Output
The program generate reports in out put directory. If an input experiment description has more the one scene, all reports will be separated into subdirectories by scene, where the number directory corresponds to a scene number in the input array.
Output report for each scene may contain files:
* `TextReport.txt` - text description some experiment parameters with some result statistics;
* `Detextord.dat` - description position of detectors according to [format requirement](http://grapherhelp.goldensoftware.com/WTOPICS/TOP_ASCIIFILES.htm);
* `Distance.grd` - description errors in distance in signal localization on the scene according to [format requirement](http://grapherhelp.goldensoftware.com/subsys/ascii_grid_file_format.htm);
* `Time.grd` - description errors in time in signal localization on the scene according to [format requirement](http://grapherhelp.goldensoftware.com/subsys/ascii_grid_file_format.htm).

All formats were chosen for possibility to visualisation data in [Surfer®](https://www.goldensoftware.com/products/surfer).
