#include "tool.h"


/**
 * remove duplicates for QStringList
 *
 * @author hzc
 * @param that qstringlist object
 * @return number of string removed
 */
int Tool::QStringList_removeDuplicates(QStringList *that){
    int n = that->size();
    int j = 0;
    QSet<QString> seen;
    seen.reserve(n);
    int setSize = 0;
    for (int i = 0; i < n; ++i) {
        const QString &s = that->at(i);
        seen.insert(s);
        if (setSize == seen.size()) // unchanged size => was already seen
            continue;
        ++setSize;
        if (j != i)
            that->swap(i, j);    //将不重复项与重复项交换
        ++j;
    }
    if (n != j)
        that->erase(that->begin() + j, that->end());
    return n - j;

}
