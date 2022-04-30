from cmath import cos, sin
import xml.etree.cElementTree as Tree
world = Tree.Element('world')


def calculate_circle(raio):
    points = []

    nrPoints = 8

    angle = 360 / 8

    for i in range(8):
        point = (cos(angle*i)*raio,sin(angle*i)*raio)
        points.append(point)

    return points

points = calculate_circle(5.5)

for p in points:
    point = Tree.SubElement(world, "point")
    point.attrib["x"]=str(p[0].real)
    point.attrib["y"]="0"
    point.attrib["z"]=str(p[1].real)

tree = Tree.ElementTree(world)

tree.write('curvas.xml')