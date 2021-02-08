# How to use the program?
You can run this program from terminal with next input arguments.
## Input arguments:
* `experiment` (short name: `e`, required) - path to JSON file with experiment description;
* `out` (short name: `o`, default: `""`) - output directory path;
* `time` (short name: `t`, default: `false`) - flag for analyses time errors in experiment;
* `distance` (short name: `d`, default: `false`) - flag for analyses distance errors in experiment;
* `text_report` (short name: `r`, default: `false`) - flag for creation text description of an experiment;
* `help` (short name: `h`, default: `false`) - return help message.

Arguments `experiment` and `out` have implicit setting, if place they value at first and second position of arguments.

## Example start commands:  
Next command conducts an experiment described in `"expirement.json"` file, save result, describing distance errors, and text report in `"out"` directory.  
`PLoc_Modeling --experiment="expirement.json" --out="out" --distance --text_report`  
It same with:  
`PLoc_Modeling "expirement.json" "out" -d -r`  
or:  
`PLoc_Modeling "expirement.json" "out" -dr`

## Experiment description format:
Experiments described in JSON format. In description use next types:

### Point
Position of something. Contains fields:
* `x` (_double_) - first coordinate of point;
* `y` (_double_) - second coordinate of point.

### Field
Describes a field for experiment. Contains fields:
* `min` (_Point_) - minimal values of coordinates in a field;
* `max` (_Point_) - maximal values of coordinates in a field;
* `detectors` (_[Points]_) - array of detectors position.

### Grid
Contains fields:
* `width` (_int_) - width a grid;
* `height` (_int_) - height a grid.

### ErrorGenerator
Describes random error generator. Contains fields:
* `type` - type of generator. Can take the following values:
    * `Linear` - for generator with linear distribution;
    * `Normal` - for generator with normal distribution;
* Fields describing the generator.
    
#### Linear ErrorGenerator fields
* `min_value` (_double_) - low generator border;
* `max_value` (_double_) - upper generator border.

#### NormalErrorGenerator fields
* `mean` (_double_) - mean of normal distribution;
* `standard_deviation` (_double_) - standard deviation of normal distribution.

### AlgorithmType
Describes localization algorithm type. Can contain next values: `"Direct"`, `"Quadrangle"`, `"Vector"`.

### CombinerType
Describes algorithm of combiner results. Can contain next values: `"Mean"`.

### Experiment description
Describe an experiment. Path to file with this JSON object must be specified on program start. Contains fields:
* `fields` (_[Field / String]_) - array of objects describing fields for an experiment. Can contain Field or string dates. In string must be specified a path to file with field structure. 
* `grid` (_Grid_) - sampling grid of field;
* `number_attempts` (_unsigned int_) - number attempts of localization in each node of the grid;
* `c_error_generator` (_ErrorGenerator_) - generator errors in wave propagation speed;
* `time_error_generator` (_ErrorGenerator_) - generator errors in time detection;
* `algorithm` (_AlgorithmType_) - localization algorithm type;
* `combiner` (_CombinerType_) - combiner results algorithm type.
