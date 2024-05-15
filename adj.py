import numpy as np

def generate_adjacency_matrix(n):
    # Initialize an n x n matrix with zeros
    adjacency_matrix = np.zeros((n, n), dtype=int)
    
    # Generate random edges for the graph
    for i in range(n):
        for j in range(i+1, n):
            # Randomly decide if there's an edge between nodes i and j
            edge = np.random.randint(0, 2)
            # Assign the edge to both symmetric positions in the matrix
            if (i!=j) :
                adjacency_matrix[i][j] = edge
                adjacency_matrix[j][i] = edge
            else:
                 adjacency_matrix[i][j] = 0;
                 adjacency_matrix[j][i] = 0;
    
    return adjacency_matrix

# Example usage:
n = 400  # Number of nodes
adj_matrix = generate_adjacency_matrix(n)

# Write the output to a file
file_name = "adjacency_matrix.txt"
with open(file_name, "w") as file:
    file.write("Adjacency Matrix:\n")
    file.write("{\n")
    for i in range(0,n):
        file.write("{")
        for j in range(0,n):
            file.write(f"{adj_matrix[i][j]}")
            if j != n-1:
                file.write(",")
        if i == n-1:
            file.write("}\n")
        else:
            file.write("},\n")
    file.write("}")
    
print(f"Adjacency matrix has been saved to '{file_name}'")
