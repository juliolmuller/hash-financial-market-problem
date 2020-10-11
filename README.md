<h1 align="center">
  :hash: The Trading Company Problem
</h1>

- **Project proposed by:** [Lucila Bento, Vinícius de Sá & Jayme Szwarcfiter](https://www.academia.edu/20715778/Hashing_na_solu%C3%A7%C3%A3o_de_problemas_at%C3%ADpicos)
- **Project developed by:** [Julio L. Muller](https://github.com/juliolmuller)
- **Released on:** Out 5, 2020
- **Updated on:** Out 5, 2020
- **Latest version:** 1.0.0
- **License:** MIT

The main objective of this project is to develop a solution in C language to the **Problema do Mercado Financeiro** (*Trading Company Problem*) using the *hash table* data structure. This was based on a [brazilian paper](https://www.academia.edu/20715778/Hashing_na_solu%C3%A7%C3%A3o_de_problemas_at%C3%ADpicos) with other 3 problems that can be solved with hash tables.

Consider the following hypothetical problem: A certain company **X** operates in the Stocks Exchange market, inputting a huge number of financial transactions - called trades - every day. On the other side of each trade, there is some company **Y**, call it a partner of **X**. To each trade of **X** is assigned a portfolio number, which is not a unique identifier of the trade, and is not even unique per partner. In other words, trades with different partners may be assigned the same portfolio, and different trades with the same partner may be assigned distinct portfolios as well, according to some internal policy of the company. Whenever **X** enters into a trade, a new line such as "`+ 101 25`" is appended to a log file, where "`+`" indicates that a new trade was initiated by **X**, "`101`" is the partner ID and "`26`" is the portfolio of that trade. At any time during a typical day, however, a partner **Y** may withdraw all transactions with **X**. From **X**'s standpoint, that means all trades it had entered into with **Y** that day are now considered void, and a line such as "`- 101`" is logged. Here, the "`-`" sign indicates that a cancellation took place, and "`101`" is the ID of the partner the cancellation refers to. The following table illustrates the basic structure of the log file used by **X**:

|Tipo|Parceiro|Portfólio|
|:--:|:------:|:-------:|
| +  |     101|   145113|
| +  |       2|       26|
| +  |     101|       26|
| +  |    3005|     4550|
| -  |     101|         |
| +  |    3005|   145113|
| +  |       4|      184|
| +  |     101|     4550|
| -  |       2|         |

By the end of each day, the company **X** needs to determine the set of distinct portfolios associated to trades that are still active by then, that is, the portfolios of trades that have not been canceled during the day. What is the most efficient way to achieve that?

### Hash Set Strategy

As usual, it is easy to think of different ways to process the file and come up with the desired information. For example, we can traverse the file in a top-down fashion (in chronological order), and, for each line "`+ Y P`", we check whether there is any line below it (logged after it) indicating the cancellation of previous trades with **Y**. In case there is no such line, add P to a list of active portfolios, taking care not to add the same portfolio twice. To avoid duplicates, one can already think of using a hash set, where portfolios are inserted only if they are not there already, something that can be verified in constant time, on average. This approach surely works, and it is easy to see that the time complexity of this strategy is *O*(n²), where `n` denotes the number of lines in the log file.

Yet the file can be really huge, and determining the active portfolios is a time-critical operation for **X**. A quadratic-time algorithm is not good enough.

### Two Mirroring Hash Maps Strategy

Consider the use of two hash maps, this time: (I) The first one will hold a partners-by-portfolio map: the keys are portfolio numbers, and the value stored with each key **P** is a list containing the partners **Y** of **X** for which there are active negotiations under that portfolio **P**; (II) The second is a portfolios-by-partner hash map: the keys are the partners of **X**, and the values associated with each partner **Y** is a list containing the portfolios assigned to active negotiations with that partner.

The file is traversed top-down. Each line with a "`+`" sign, be it "`+ Y P`", triggers two updates in our data structure: the first is the inclusion of **Y** in the list of partners associated with portfolio **P** in the partners-by-portfolio hash map (the key **P** will be inserted for the first time if it is not already there); the second change is the inclusion of **P** in the list of portfolios associated with partner **Y** in the portfolios-by-partner hash map (the key **Y** will be first inserted for the first time if it is not already there).

Whenever a line with a "`-`" sign is read, be it "`- Y`", we must remove all occurrences of **Y** from both tables. In the second, portfolios-by-partner hash map, **Y** is the search key, hence the bucket associated to **Y** can be determined directly by the hash function, and the list of portfolios associated to **Y** can be retrieved (and deleted, along with **Y**) in average time *O*(1). In partners-by-portfolio, however, **Y** can belong to lists associated to several distinct portfolios. To avoid traversing the lists of all portfolios, we can use the information in the first, portfolios-by-partner map (sure enough, before deleting the key **Y**) so we know beforehand which portfolios will have **Y** on their partner lists. We may thus go directly to those lists and remove **Y** from them, without the need to go through the entire table. If the list of partners associated to a portfolio **P** becomes empty after the removal of **Y**, then we delete the portfolio **P** from that first hash map altogether.

After all lines of the file have been processed, just return the portfolios corresponding to keys that remain in the first partners-by-portfolio hash map. The figure below illustrates the data structures used in this solution. Note that, for clarity, different keys in the same bucket (a collision in the hash table) are not being even considered. As already mentioned, collisions (as well as the choice of the hash function and other technicalities) are being taken care of by the lower level hash implementation, allowing us to focus on the high level usage of it, as happens most of the time in practice.

![Illustrative figure](https://www.scielo.br/img/revistas/pope/v35n2//1678-5142-pope-35-02-00423-gf2.jpg)

Determining the lists from which a certain partner **Y** must be removed (due to a cancellation read from the file) can be made in average *O*(1) time per list, and each partner can be removed at most once for each line of the file that inserted it there. So far so good. However, in order to find **Y** in each list (something we also need to do by the time we insert **Y**, so we do not have the same partner appear more than once in the same list), we still need to traverse the whole list, which certainly increases the computational time. Since we wish we can cope with the whole file processing task in linear time, we must think of a remedy to this.

### Multi-level Hashing Strategy

So our problem is the time-consuming task of traversing whole lists to locate a single element. We therefore want to try and replace those lists with more performable structures, such as binary search trees, or, better yet... hash sets! In other words, the value associated with each key **P** in the partners-by-portfolio hash map will be, instead of a list, a hash set whose keys are the IDs of the partners associated with **P**. The same goes for the second, portfolios-by-partner hash map, where portfolios will be stored in a hash set associated to each partner **Y**. Thus, partners and portfolios can be included and removed without the need to traverse possibly lengthy lists, but in average constant time instead. Since the cost of processing each line is now *O*(1) (due to a constant number of hash table dictionary operations being performed), the whole algorithm, implemented this way, runs in expected linear time in the size of the file. (Sure enough, the same time complexity could have been achieved, in the previous approach, by making a cumbersome use of pointers across the hash tables.)

![Illustrative figure](https://www.scielo.br/img/revistas/pope/v35n2//1678-5142-pope-35-02-00423-gf3.jpg)

### Bottom-up traversal :star: (bare creativity)

The most elegant solution for the Trading Company Problem though consists in reading the log file in a bottom-up fashion (in anti-chronological order), avoiding the need to process trades that would be, later in the day, canceled anyway.

Note that, as the file is being read upwards, the first line found with a "`-`" sign, be it "`- Y`", indicates the cancellation of all trades company **X** had entered into with partner **Y** earlier in the day. Thus, such a line **L<sub>Y</sub>** in the log file partitions all other lines associated to **Y** into two groups: those which occurred chronologically before **L<sub>Y</sub>** (and which have not been processed yet, since they appear above it in the file), and those which occurred chronologically after **L<sub>Y</sub>** (and therefore have already been processed). From that moment on, the lines of the former group may be simply ignored, since the corresponding trades would not be active by the end of the day anyway. On the other hand, all lines regarding **Y** that have already been processed (those in the latter group) correspond to trades that are active indeed by the end of the day, since they appear, in the log file, chronologically after the last cancellation involving **Y**.

We can therefore use again two hash sets: one for the **active portfolios**, whose keys will be returned by the end of the algorithm execution, and another for the **bypassed partners**, that is, the partners for which a cancellation line has already been read in the bottom-up traversal of the file. With such a simple structure, each "`- Y`" line triggers the insertion of **Y** into the bypassed partners hash set (if it is not already there), and each "`+ Y P`" line either triggers the insertion of **P** in the active portfolios hash set, in case it is not already there and **Y** is not in the bypassed partners table. Otherwise the line is ignored.

This time again each line gives rise to a constant number of average constant-time operations, but in a much simpler fashion. The whole process runs in *O*(n) time, where `n` is the number of lines in the log file.

## :trophy: Lessons Learned

- **Hash Tables** data structure;

## :hammer: Technologies & Resources

- *C* language
- Minimalist GNU for Windows

## :bell: Compiling and Running the Program

To execute the program, first you need to compile its source code using a **C compiler**. As I am on Windows, I'm using *[MinGW](http://www.mingw.org/)* that allows me to use `gcc` for compilation:

```bash
# Compiles the application to "tc.exe"
$ gcc main.c -o tc

# Run the application
$ tc

# Run the application using files as standard input
$ tc < test1.txt
```
