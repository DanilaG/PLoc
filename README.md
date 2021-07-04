# PLoc.
It is the set of tools for passive radio location.
PLoc contains localization functions and the program for modeling the localization of a signal emitter.
[Read more.](https://drive.google.com/file/d/1T6fNNqaKZ2Wo6I8oi54oja0lyv3S-_BZ/view?usp=sharing)

## Localization methods.
PLoc has three directs localization methods:
* Direct;
* Quadratic Planar (QP);
* Rectangle;


and one approximate method:
* Elder-Mead.

In practice, Elder-Mead gives the best answers but requires too much time. So, you can calculate the answer by QP or Rectangle method and clarify the result by Elder-Mead.

Direct methods work with a certain number of detectors (three detectors in Direct and QP methods and four in Rectangle).
But they can be scaled to a large number by localization for each locator's combination. In this way, we need to combine the results.

## Combiners
PLoc have five result combine methods:
* Mean;
* Filtered Mean (mean with discarding bursts);
* Median;
* Time Sum (give priority combination with minimal time detection);
* Triangle (only for Direct and QP, give priority more regular triangles).

In practice, Rectangle method gives the best results with Median combiner. For other methods don't have a favorite combiner.

## Modeling
PLoc has a program for the modeling localization process. With their help, you can choose the bests position for your detectors and method of localization.
[Read more about the program.]()

Also, you can use scripts that were written on Python for partly visualization of results. 


## Examples
Examples of localization are with added errors.  
<p align="center">
    <img src="https://i.ibb.co/VmGhs0k/field-dist.jpg" width="300"></br>
    Field of mean distance errors. Localization by QP methods on plaint.</br>
    </br>
    <img src="https://i.ibb.co/ZSvz54p/Localization-spread-1.png" width="300"></br>
    Spread of lighting localization with curce in the center of the triangle of stations.</br>
    </br>
    <img src="https://i.ibb.co/gmw8QXT/Localization-spread-time.png" width="400"></br>
    Spread of time lighting's appearance with source in the center of the triangle of stations.</br>
    </br>
    <img src="https://i.ibb.co/kX50fQJ/H.png" width="500"></br>
    Field of mean distance errors. Localization by Elder-Mead on sphere.</br>
</p>