
# NYCU_PDA_Lab1 
This repository contains the implementation of Lab 1 from the **Physical Design Automation (PDA)**  course at National Yang Ming Chiao Tung University (NYCU). The lab focuses on implementing the **Corner Stitching**  data structure, which is a technique for managing VLSI layout efficiently.

---


## Table of Contents 
 
1. [Introduction](https://chatgpt.com/c/674018e5-e898-8001-94b9-f3dee2ec3f9e#introduction)
 
2. [Features](https://chatgpt.com/c/674018e5-e898-8001-94b9-f3dee2ec3f9e#features)
 
3. [Usage](https://chatgpt.com/c/674018e5-e898-8001-94b9-f3dee2ec3f9e#usage)
 
4. [Visualization](https://chatgpt.com/c/674018e5-e898-8001-94b9-f3dee2ec3f9e#visualization)
 
5. [References](https://chatgpt.com/c/674018e5-e898-8001-94b9-f3dee2ec3f9e#references)


---


## Introduction 
The goal of this lab is to manage space tiles as **maximal horizontal stripes**  using the Corner Stitching technique. Each space tile should have no other space tiles  immediately to its right or left and every space tile must be as wide as possible.

### Problem Overview 

- The input describes the layout's outline, block creation commands, and point-finding queries.
 
- The output includes:
  - Total tiles in the layout.

  - Adjacency information for each block.

  - The location of specific points based on the input.
For detailed specifications and examples, please refer to the provided `2024PDA_Lab1.pdf` and `Lab1_Supplementary.pdf`.

---


## Features 
 
1. **Efficient Implementation:**  Uses the Corner Stitching data structure to organize and process VLSI layout tiles.
 
2. **Input and Output Management:**  
  - Input data resides in the `./testcase` directory.
 
  - Output files, including example solutions, are available in the `./output` directory.
 
3. **Visualization:**  Provides tools for visualizing the layout with block and space tiles.


---


## Usage 

### Compilation 
Compile the project using the provided `Makefile`:

```bash
make
```
This will produce an executable named `Lab1`.
### Execution 

Run the program using the following command:


```bash
./Lab1 [input_file] [output_file]
```
 
- Example:

```bash
./Lab1 ./testcase/case0.txt ./output/output0.txt
```

### Visualizing the Layout 

Use the provided Python script to generate visual representations of the layout:


```bash
python3 draw_block_layout.py [drawing_file] [output_image]
```
 
- Example:

```bash
python3 draw_block_layout.py ./drawing/layout0.txt ./drawing/layout0.png
```


---


## Visualization 
Visualizing the layout is an optional but helpful step to debug and validate the implementation. Example images are included in the `./drawing` directory. The script outputs layouts as `.png` files. 
- **Command Example:** 

```bash
python3 draw_block_layout.py ./drawing/example.txt ./drawing/output.png
```


---


## References 

The implementation of the Corner Stitching data structure is based on the paper:
 
- **J. K. Ousterhout** , *"Corner Stitching: A Data-Structuring Technique for VLSI Layout Tools"*

This paper outlines the foundational techniques used to develop efficient VLSI layout tools and serves as the primary reference for this project.
