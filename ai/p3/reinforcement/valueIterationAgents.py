# valueIterationAgents.py
# -----------------------
# Licensing Information:  You are free to use or extend these projects for
# educational purposes provided that (1) you do not distribute or publish
# solutions, (2) you retain this notice, and (3) you provide clear
# attribution to UC Berkeley, including a link to http://ai.berkeley.edu.
# 
# Attribution Information: The Pacman AI projects were developed at UC Berkeley.
# The core projects and autograders were primarily created by John DeNero
# (denero@cs.berkeley.edu) and Dan Klein (klein@cs.berkeley.edu).
# Student side autograding was added by Brad Miller, Nick Hay, and
# Pieter Abbeel (pabbeel@cs.berkeley.edu).


import mdp, util

from learningAgents import ValueEstimationAgent

class ValueIterationAgent(ValueEstimationAgent):
    """
        * Please read learningAgents.py before reading this.*

        A ValueIterationAgent takes a Markov decision process
        (see mdp.py) on initialization and runs value iteration
        for a given number of iterations using the supplied
        discount factor.
    """
    def __init__(self, mdp, discount = 0.9, iterations = 100):
        """
          Your value iteration agent should take an mdp on
          construction, run the indicated number of iterations
          and then act according to the resulting policy.
        """
        self.mdp = mdp
        self.discount = discount
        self.iterations = iterations
        self.values = util.Counter()

        for i in range(iterations):
            prime_values = self.values.copy()
            for s in mdp.getStates():
                # Iteration 1 - value is the state's reward
                # If a state is a terminal state its value is its reward
                if i == 0 or mdp.isTerminal(s):
                    self.values[s] = mdp.getReward(s, None, None)
                    continue
                # Consider each action to find max expected value
                values = []
                for a in mdp.getPossibleActions(s):
                    tran_states = mdp.getTransitionStatesAndProbs(s, a)
                    exp_value = sum(prime_values[s_prime] * prob for s_prime,prob in tran_states)
                    value = mdp.getReward(s, a, None) + self.discount * exp_value
                    values.append(value)
                self.values[s] = max(values)

    def getValue(self, state):
        """
          Return the value of the state (computed in __init__).
        """
        return self.values[state]


    def computeQValueFromValues(self, state, action):
        """
          Compute the Q-value of action in state from the
          value function stored in self.values.
        """
        reward = self.mdp.getReward(state, action, None)
        tran_states = self.mdp.getTransitionStatesAndProbs(state, action)
        return sum(prob * (reward + self.discount * self.values[s_prime]) \
                   for s_prime,prob in tran_states)

    def computeActionFromValues(self, state):
        """
          The policy is the best action in the given state
          according to the values currently stored in self.values.

          You may break ties any way you see fit.  Note that if
          there are no legal actions, which is the case at the
          terminal state, you should return None.
        """
        if self.mdp.isTerminal(state):
            return None
        actions = self.mdp.getPossibleActions(state)
        q_values = list(map(lambda a: (a, self.computeQValueFromValues(state, a)), actions))
        return max(q_values, key=lambda x: x[1])[0]

    def getPolicy(self, state):
        return self.computeActionFromValues(state)

    def getAction(self, state):
        "Returns the policy at the state (no exploration)."
        return self.computeActionFromValues(state)

    def getQValue(self, state, action):
        return self.computeQValueFromValues(state, action)
