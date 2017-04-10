from Week2.nnPy import Input, Sigmoid, Linear
from Week2.nnPy import topological_sort, forward_pass
import numpy as np

if __name__ == "__main__":
    inputs = Input()
    weights = Input()
    bias = Input()

    f = Linear(inputs, weights, bias)
    g = Sigmoid(f)

    x = np.array([[-1., -2.], [-1, -2]])
    w = np.array([[2., -3], [2., -3]])
    b = np.array([-3., -5])

    feed_dict = {inputs: x, weights: w, bias: b}

    graph = topological_sort(feed_dict)
    output = forward_pass(g, graph)

    """
    Output should be:
    [[  1.23394576e-04   9.82013790e-01]
     [  1.23394576e-04   9.82013790e-01]]
    """
    print(output)
