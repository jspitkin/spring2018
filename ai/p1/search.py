# search.py
# ---------
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


"""
In search.py, you will implement generic search algorithms which are called by
Pacman agents (in searchAgents.py).
"""

import util

class SearchProblem:
    """
    This class outlines the structure of a search problem, but doesn't implement
    any of the methods (in object-oriented terminology: an abstract class).

    You do not need to change anything in this class, ever.
    """

    def getStartState(self):
        """
        Returns the start state for the search problem.
        """
        util.raiseNotDefined()

    def isGoalState(self, state):
        """
          state: Search state

        Returns True if and only if the state is a valid goal state.
        """
        util.raiseNotDefined()

    def getSuccessors(self, state):
        """
          state: Search state

        For a given state, this should return a list of triples, (successor,
        action, stepCost), where 'successor' is a successor to the current
        state, 'action' is the action required to get there, and 'stepCost' is
        the incremental cost of expanding to that successor.
        """
        util.raiseNotDefined()

    def getCostOfActions(self, actions):
        """
         actions: A list of actions to take

        This method returns the total cost of a particular sequence of actions.
        The sequence must be composed of legal moves.
        """
        util.raiseNotDefined()


def tinyMazeSearch(problem):
    """
    Returns a sequence of moves that solves tinyMaze.  For any other maze, the
    sequence of moves will be incorrect, so only use this for tinyMaze.
    """
    from game import Directions
    s = Directions.SOUTH
    w = Directions.WEST
    return  [s, s, w, s, w, w, s, w]

def depthFirstSearch(problem):
    """
    Search the deepest nodes in the search tree first.

    Your search algorithm needs to return a list of actions that reaches the
    goal. Make sure to implement a graph search algorithm.

    To get started, you might want to try some of these simple commands to
    understand the search problem that is being passed in:
    """

    frontier = util.Stack()
    start = problem.getStartState()
    frontier.push(start)
    # trace: node label -> (parent label, action)
    trace = { start : (None, None) }
    explored = set()

    while not frontier.isEmpty():
        cur = frontier.pop()
        if problem.isGoalState(cur):
            return construct_path(trace, cur)
        explored.add(cur)
        for child in problem.getSuccessors(cur):
            if child[0] not in explored:
                trace[child[0]] = (cur, child[1])
                frontier.push(child[0])
    return []


def breadthFirstSearch(problem):
    """Search the shallowest nodes in the search tree first."""
    
    frontier= util.Queue()
    start = problem.getStartState()
    frontier.push(start)
    # trace: node label -> (parent label, action)
    trace = { start : (None, None) }
    explored = set([start])

    while not frontier.isEmpty():
        cur = frontier.pop()
        if problem.isGoalState(cur):
            return construct_path(trace, cur)
        for child in problem.getSuccessors(cur):
            if child[0] in explored:
                continue
            frontier.push(child[0])
            explored.add(child[0])
            trace[child[0]] = (cur, child[1])
    return []


def uniformCostSearch(problem):
    """Search the node of least total cost first."""

    frontier  = util.PriorityQueue()
    start = problem.getStartState()
    frontier.push(start, 0)
    # trace: node label -> (parent label, action)
    trace = { start : (None, None) }
    path_cost = { start : 0 }
    explored = set()

    while not frontier.isEmpty():
        cur = frontier.pop()
        if problem.isGoalState(cur):
            return construct_path(trace, cur)
        explored.add(cur)
        for child in problem.getSuccessors(cur):
            if child[0] not in explored:
                cost = path_cost[cur] + child[2]
                if child[0] not in path_cost or cost < path_cost[child[0]]:
                    path_cost[child[0]] = cost
                    trace[child[0]] = (cur, child[1])
                    frontier.update(child[0], cost)
    return []
    

def nullHeuristic(state, problem=None):
    """
    A heuristic function estimates the cost from the current state to the nearest
    goal in the provided SearchProblem.  This heuristic is trivial.
    """
    return 0

def aStarSearch(problem, heuristic=nullHeuristic):
    """Search the node that has the lowest combined cost and heuristic first."""

    frontier = util.PriorityQueue()
    start = problem.getStartState()
    g = { start : 0 }
    explored = set()
    # trace: node label -> (parent label, action)
    trace = { start : (None, None) }
    frontier.push(start, g[start] + heuristic(start, problem))
    while not frontier.isEmpty():
        cur = frontier.pop()
        if problem.isGoalState(cur):
            return construct_path(trace, cur)
        explored.add(cur)
        for child in problem.getSuccessors(cur):
            if child[0] not in explored:
                cost = g[cur] + child[2] + heuristic(child[0], problem)
                if child[0] not in g or cost < g[child[0]]:
                    g[child[0]] = g[cur] + child[2]
                    trace[child[0]] = (cur, child[1])
                    frontier.update(child[0], cost)
    return []

def construct_path(trace, goal):
    cur_node = goal
    cur_action = trace[goal][1]
    path = []
    while trace[cur_node][0] is not None:
        path.append(cur_action)
        cur_node = trace[cur_node][0]
        cur_action = trace[cur_node][1]
    path.reverse()
    return path

# Abbreviations
bfs = breadthFirstSearch
dfs = depthFirstSearch
astar = aStarSearch
ucs = uniformCostSearch
