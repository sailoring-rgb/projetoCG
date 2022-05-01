from cmath import cos, sin
import math
import xml.etree.cElementTree as Tree
world = Tree.Element('world')

raio = 2.0

def calculate_circle(raio):

    points = []

    nrPoints = 8

    i = 0
    for i in range(nrPoints):
        angle = math.pi / 4
        if i == 0 or i == 4:
            point = (math.cos(angle*i)*raio,0)
        elif i == 2 or i == 6:
            point = (0,math.sin(angle*i)*raio)
        else: point = (math.cos(angle*i)*raio,math.sin(angle*i)*raio)
        points.append(point)

    return points

points = calculate_circle(raio)

for p in points:
    point = Tree.SubElement(world, "point")
    point.attrib["x"]=str(p[0].real)
    point.attrib["y"]="0.3"
    point.attrib["z"]=str(p[1].real)

tree = Tree.ElementTree(world)

tree.write(f'curvas{raio}.xml')