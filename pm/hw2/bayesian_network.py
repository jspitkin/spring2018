import pandas as pd

def productFactor(A, B):
    """ Computes the factor between A and B and returns the result.
        Assumes the product of A and B is a valid operation.
    """
    C = reshape(A, B)
    D = reshape(B, A)
    # Compute the probabilities
    for index, row in C.iterrows():
        C.loc[index, 'probs'] = C.loc[index, 'probs'] * D.loc[index, 'probs']
    print(C)
    return C

def get_prob(A, var_list, value_list):
    for A_index, row, in A.iterrows():
        found = True
        for index, var in enumerate(var_list):
            if A.loc[A_index, var] != value_list[index]:
                found = False
                break
        if found:
            return A.loc[A_index, 'probs']
    return -1

def reshape(A, B):
    A_vars = A.columns.values.tolist()[1:]
    B_vars = B.columns.values.tolist()[1:]
    product_vars = list(set(A_vars).union(set(B_vars)))
    # Create a data frame of the proper shape for the product
    level_list = []
    rows = 1
    for var in product_vars:
        if var in A_vars:
            level_list.append(A[var].unique())
            rows *= len(A[var].unique())
        else:
            level_list.append(B[var].unique())
            rows *= len(B[var].unique())
    probabilities = [0] * rows
    C  = createCPT(product_vars, probabilities, level_list)
    for index, row in C.iterrows():
        rel_values = []
        for var in A_vars:
            rel_values.append(C.loc[index, var])
        C.loc[index, 'probs'] = get_prob(A, A_vars, rel_values)
    return C

def marginalizeFactor(A, margVar):
    """ Marginalizes margVar from a single factor A.
        Assume that margVar appears on the left side of the conditional.
    """
    return 0

def marginalize(bayesNet, margVars):
    """ Takes in a bayesNet and marginalizes out all variables in margVars.
        This is done using variable elimination.
    """
    return 0

def observe(bayesNet, obsVars, obsVals):
    """ Takes in a bayesNet and and sets the list of variables obsVars
        to the corresponding list of values obsVals. The factors are
        not normalized as probabilities.
    """
    return 0

def infer(bayesNet, margVars, obsVars, obsVals):
    """ Takes in a bayesNet and returns a single joint proability table
        resulting from observing a set of variables and marginalizing a
        set of variables. The values in the table are normalized as
        probabilities.
    """
    return 0 

def createCPT(varnames, probs, levelsList):
    cpt = pd.DataFrame({'probs': probs})

    m = len(probs)
    n = len(varnames)

    k = 1
    for i in range(n - 1, -1, -1):
        levs = levelsList[i]
        numLevs = len(levs)
        col = []
        for j in range(0, numLevs):
            col = col + [levs[j]] * k
        cpt[varnames[i]] = col * int(m / (k * numLevs))
        k = k * numLevs

    return cpt
