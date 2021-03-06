#! /usr/bin/env python

from hermes2d import Mesh, H1Shapeset, PrecalcShapeset, H1Space, \
        BaseView

from hermes2d.forms import set_forms
from hermes2d.examples import get_example_mesh
domain_mesh = get_example_mesh()

mesh = Mesh()
mesh.load(domain_mesh)
mesh.refine_element(0)
shapeset = H1Shapeset()
pss = PrecalcShapeset(shapeset)

# create an H1 space
space = H1Space(mesh, shapeset)
space.set_uniform_order(5)
space.assign_dofs();

bview = BaseView()
bview.show(space)
bview.wait()
