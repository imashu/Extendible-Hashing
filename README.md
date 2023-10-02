# Extendible-Hashing
In this assignment, you are required to implement Extendible Hashing. Specialized libraries for managing hash tables
must not be used. You may use existing libraries for basic data structures like vectors and associative array (e.g, map in
C++ STL libraries or equivalent in JAVA). You may also use math libraries as needed for tasks like, generating random
numbers, converting to and from binary format, etc.
# Dataset creation:
For this question, you would have to create a synthetic table (simulating sales records of department stores) containing 1
Lakh records. Each record in this file contains four fields: (1) Transaction ID (an integer), (2) Transaction sale amount (an
integer), (3) Customer name (string) and, (4) category of item. Transaction ID is an integer to identify each transaction
uniquely in the dataset. Transaction sale amount is a random integer between 1 and 500000. Customer name is a random 3
letter string. You can model this as a character array of length 3. Category of the item is a random integer between 1
--1500 (uniformly distributed across the table).
After creating the dataset, you are required to store the dataset in an Extendible hash. Implementation
specification of the extendible hash is given below:
# Details of Extendible Hash:
● We would hash on “TransactionID.”
● Ideally, the buckets in the extendible hash should be stored in the secondary memory. However, for the purpose of
this project, they would be stored in something called “Simulated Secondary Memory (detailed below).”
● The directory or bucket address table of the extendible hash would contain hash prefix and a pointer to the bucket
sitting in “Simulated Secondary Memory (detailed below).”
Simulated Secondary Memory:
The following tips would help you achieve this.

(a) The secondary memory can be simulated through an array of the abstract data-type “bucket”. You can fix a very
large size for this array.

(b) simulated secondary memory must be an array only. No arraylist, vector, and dynamic arrays are allowed.

(c) The bucket capacity is fixed in terms of the number of index records it can contain. Do not hard code this number
as it would be varied in the experiments.

(d) Indices in this array form our “bucket address/hardware address.”

(e) Here, the bucket abstract data type would have the following information:
a. Number of empty spaces
b. An array of structures to store the index records. Length of this array is fixed according to the parameter
“bucket-size” specified.
c. Link to the next bucket (valid only if this bucket is overflowing)
d. All buckets in the overflow chain must be linked. The last bucket of the overflow chain must have a
special character denoting that it is the end of the overflow chain.


(f) Note that in your entire code, there should be only one abstract data type for the bucket for all the
purposes, viz., records and overflow buckets.

(g) It is advisable to keep a separate area in the secondary memory for storing the overflow buckets.

(h) There should be only one instance of secondary memory running in your code.
Main Memory

(a) You can assume to have enough “main memory” for “bringing” in the buckets to be rehashed.
# Other Details:

(a) The most significant bits are extracted to find the directory entry.

(b) Only one directory expansion is allowed per record insertion. Following the directory expansion, you may attempt
to resolve the collision (if it still persists) by increasing the local depth (if local depth < global depth). In case the
collision is still not resolved, just create an overflow bucket.

(c) During an insert operation if there is a space in the overflow bucket associated with a has prefix then no need to
call for any directory expansion and/or local depth increase.
