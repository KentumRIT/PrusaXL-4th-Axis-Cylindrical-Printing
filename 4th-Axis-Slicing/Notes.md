# THIS DOCUMENTATION IS TO BE USED AS A LAB JOURNAL, WITH ALL EXPERIMENTS BOTH SUCCESSES AND FAILURES BEING RECORDED
# Goal
We need to generate g-code that is planar with respect to a cylindrical coordinate system, i.e. building concentric rings around the mandrel to produce a final shape. However the g-code is managed, it must account for the radius-dependent relationship between angular position and velocity and linear surface position and velocity.

# Things I Found
- Generalized non-planar slicer: [video](https://www.youtube.com/watch?v=VEgwnhLHy3g), [slicer](https://github.com/jyjblrd/S4_Slicer)
- Non-planar slicing with planar slicer: [article](https://xyzdims.com/2021/04/10/3d-printing-non-planar-slicing-with-planar-slicer/)
- Cylindrical slicing: [paper](https://www.sciencedirect.com/science/article/pii/S2212827117311472?via%3Dihub)
- Planar cylindrical slicing, (flat to curved) [paper](https://accscience.com/journal/IJB/articles/online_first/4756)
- Company that does 4th axis cylindrical printing [link](https://www.cosineadditive.com/en/blog/2022/12/15/additive-lathe-slicing)
- Cylindrical Slicer [github](https://github.com/vandenbergheluke/cylindrical-slicer)

# Potential Solutions
## Most recent attempted solution
### Description
In more words, what does this solution try and what's the thought process behind trying it?

### Test 1 Name (describe the test you did)
results

### Test 2 Name (descibe the test you did)
results

## Convert Flat Parts To Curved
### Description
The idea is basically to model everything on a flat plane with the y axis being the longitudinal direction of the mandrel and the x axis being the theta direction of the mandrel. In this way, we can project a complex 3D cylindrical shape onto a 2D surface. After generating g-code with that 2D shape, we can edit it such that the x-axis travel is converted to theta-axis movement.

### Pros
- No custom slicer is needed
- It may be easier to model at least some shapes in this 2D projected view

### Cons
- Annuli are a challenge: If a part forms a full ring around the mandrel, there will be walls on either side of what was the 2D part, which will create a seam in the final part
- Resolution is lost as radius increases: The slicer will generate g-code at a linear resolution reasonable for planar printing, but in cylindrical printing this resolution may be insufficient as radius increases
- Lots of g-code editing
