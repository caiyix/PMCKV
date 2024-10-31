# PMCKV
  Pipeline-based Multi-Compactions KV Stores to Improve the System Performance

# Introdction
   The Log-Structured Merge Tree (LSM-tree) in modern key-value (KV) storage systems needs compactions to move KV pairs from higher level to lower level to keep fully-sorted ordering in each level and reclaim invalid KV pairs. Unfortunately, the system performance will be hurt because read-write amplification is caused by each compaction. In this paper, we have observed that read-write amplification exists between consecutive compactions within the same level of the LSM-tree. Motivated by this, we propose the compaction in advance strategy which triggers multiple consecutive compactions ahead of time when a certain level of the LSM-tree reaches its size limit. To improve the write performance, we design a novel pipeline parallel method based on the Sorted String Table (SST) file granularity. Additionally, we apply the compaction in advance strategy and traditional compaction, with the pipeline method, for low levels and high levels of the LSM-tree, respectively. Finally, we implement a prototype system called PMCKV based on LevelDB.

# Installation

  Same as leveldb https://github.com/google/leveldb
