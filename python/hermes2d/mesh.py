from __future__ import division

import math

class ParseError(Exception):
    pass

def convert2tuple(s):
    """
    Converts any iterable to tuple recursively.
    """
    if hasattr(s, "__iter__"):
        return tuple([convert2tuple(y) for y in s])
    else:
        return s

def read_hermes_format(filename):
    """
    Reads a mesh from a file in a hermes format.

    Returns nodes, elements, boundary, nurbs or raises a ParseError if the
    syntax is invalid.
    """
    m = open(filename).read()
    return read_hermes_format_str(m)

def read_hermes_format_str(m):
    """
    Reads a mesh from a string in a hermes format.

    Returns nodes, elements, boundary, nurbs or raises a ParseError if the
    syntax is invalid.
    """
    m = m.strip()
    m = m.replace("=\n", "= \\\n")
    m = m.replace("{", "[")
    m = m.replace("}", "]")
    m = m.replace("^", "**")
    namespace = {}
    try:
        exec m in math.__dict__, namespace
    except SyntaxError, e:
        raise ParseError(str(e))
    nodes = namespace.pop("vertices", None)
    elements = namespace.pop("elements", None)
    boundary = namespace.pop("boundaries", None)
    nurbs = namespace.pop("curves", None)
    if nodes is None or elements is None or boundary is None:
        raise ParseError("Either nodes, elements or boundary is missing")
    return convert2tuple(nodes), convert2tuple(elements), \
            convert2tuple(boundary), convert2tuple(nurbs)
