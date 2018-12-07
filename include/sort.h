#ifndef SORT_H
#define SORT_H

template <typename T, typename F>
void bucket_sort(std::vector <T> &v, int maxBucket, F getBucket)
{
    std::vector < std::vector <T> > buckets(maxBucket + 5);
    for (auto &elem: v)
    {
        int ind = getBucket(elem);
        buckets[ind].push_back(elem);
    }
    v.clear();
    for (int i = 0; i < buckets.size(); i++)
    {
        for (auto &elem: buckets[i])
        {
            v.push_back(elem);
        }
    }
}

template <typename T>
void insert_sort(std::vector <T> &v, T &new_element)
{
    int it = 0;
    while (it < v.size() and v[it] < new_element)
    {
        it++;
    }
    v.insert(v.begin() + it, new_element);
}

#endif
