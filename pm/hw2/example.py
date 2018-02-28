from bayesian_network import *

def main():
    # 2016 Youtube example
    #example_1()

    # x -> y -> z
    #example_2()

    # Bishop book example
    #example_3()

    # Kevin Murphy example
    example_4()


###################################
## 2016 Example
##################################
def example_1():
    L = createCPT(['late'], [0.5, 0.5], [['T', 'F']])
    CL = createCPT(['cold', 'late'], [0.25, 0.75, 0.75, 0.25], [['T', 'F'], ['T','F']])
    FC = createCPT(['fever', 'cold'], [5/6, 2/7, 1/6, 5/7], [['T', 'F'], ['T', 'F']])
    RC = createCPT(['runny', 'cold'], [5/6, 2/7, 1/6, 5/7], [['T', 'F'], ['T', 'F']])

    coldNet = [L, CL, FC, RC]

    productFactor(L, productFactor(productFactor(CL, FC), RC))

####################################
## Simple chain example: x -> y -> z
####################################
def example_2():
    x = createCPT(['x'], [0.3, 0.7], [['T','F']])
    yx = createCPT(['y','x'], [0.8, 0.4, 0.2, 0.6], [['T','F'], ['T','F']])
    zy = createCPT(['z','y'], [0.5, 0.6, 0.5, 0.4], [['T','F'], ['T','F']])

    xyzNet = [x, yx, zy]

    ## Some simple operations you might try to check your code
    #productFactor(x, yx)
    #xyz = productFactor(productFactor(x, yx), zy)

    #marginalizeFactor(productFactor(x, yx), 'x')
    #marginalizeFactor(productFactor(yx, zy), 'z')
    #marginalizeFactor(productFactor(productFactor(zy, x), yx), 'y')

    ## Notice in the observe function, you just need to delete rows that are
    ## inconsistent with the given observations. Factors do not need to be combined
    ## or normalized in this step.
    net = observe(xyzNet, 'x', 'T')
    exit()
    #observe(xyzNet, ['x', 'y'], ['T', 'T'])

    ## Marginalize must first combine all factors involving the variable to
    ## marginalize. Again, this operation may lead to factors that aren't
    ## probabilities.
    xyzNet = marginalize(xyzNet, ['y', 'z'])
    marginalize(xyzNet, 'y')
    marginalize(xyzNet, 'z')
    marginalize(xyzNet, ['x', 'z'])

#############################
## Bishop book (Ch 8) example
#############################
def example_3():
    b = createCPT(['battery'], [0.9, 0.1], [[1, 0]])
    f = createCPT(['fuel'], [0.9, 0.1], [[1, 0]])
    gbf = createCPT(['gauge', 'battery', 'fuel'], [0.8, 0.2, 0.2, 0.1, 0.2, 0.8, 0.8, 0.9], [[1, 0], [1, 0], [1, 0]])

    carNet = [b, f, gbf]

    ## Some examples:
    ## Notice that different order of operations give the same answer
    ## (rows/columns may be permuted)
    #productFactor(productFactor(b, f), gbf)
    #productFactor(productFactor(gbf, f), b)
    #marginalizeFactor(productFactor(gbf, b), 'gauge')
    #productFactor(marginalizeFactor(gbf, 'gauge'), b)

    #productFactor(marginalizeFactor(productFactor(gbf, b), 'battery'), f)
    #marginalizeFactor(productFactor(productFactor(gbf, f), b), 'battery')

    #marginalizeFactor(productFactor(marginalizeFactor(productFactor(gbf, b), 'battery'), f), 'gauge')
    #marginalizeFactor(productFactor(marginalizeFactor(productFactor(gbf, b), 'battery'), f), 'fuel')

    ## Examples computed in book (see pg. 377)
    A = infer(carNet, ['battery', 'fuel'], [], [])      ## (8.30)
    B = infer(carNet, ['battery'], ['fuel'], [0])           ## (8.31)
    C = infer(carNet, ['battery'], ['gauge'], [0])          ## (8.32)
    D = infer(carNet, [], ['gauge', 'battery'], [0, 0]) ## (8.33)
    print(A)
    print('----')
    print(B)
    print('----')
    print(C)
    print('----')
    print(D)

###########################################################################
## Kevin Murphy's Example: http://www.cs.ubc.ca/~murphyk/Bayes/bnintro.html
###########################################################################
def example_4():
    c = createCPT(['cloudy'], [0.5, 0.5], [ ['F', 'T'] ])
    rc = createCPT(['rain', 'cloudy'], [0.8, 0.2, 0.2, 0.8], [ ['F', 'T'], ['F', 'T'] ])
    sc = createCPT(['sprinkler', 'cloudy'], [0.5, 0.9, 0.5, 0.1], [ ['F', 'T'], ['F', 'T'] ])
    wsr = createCPT(['wet', 'sprinkler', 'rain'], [1, 0.1, 0.1, 0.01, 0, 0.9, 0.9, 0.99], [ ['F', 'T'], ['F', 'T'], ['F', 'T'] ])

    grassNet = [c, rc, sc, wsr]

    A = infer(grassNet, ['cloudy', 'sprinkler'], ['wet'], ['T'])
    print(A)

if __name__ == '__main__':
    main()
## Test your infer() method by replicating the computations on the website!!
