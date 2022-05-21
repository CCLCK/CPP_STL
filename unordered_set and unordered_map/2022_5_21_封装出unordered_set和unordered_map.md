默认已经实现了开散列



用一个哈希表封装出unordered_set/unordered_map



![image-20220521230627355](https://pic-1304888003.cos.ap-guangzhou.myqcloud.com/img/image-20220521230627355.png)

为什么需要这第一个参数 因为find需要



利用模板参数KeyOfT拿到Key  比较key的地方全部替换

利用最开始写的仿函数拿到key的值映射值   

即HashFunc（Kot(T)） 取出这个类型的key的映射值





operator++  单向迭代器

如果当前结点的下一个不为空 直接访问即可

如果下一个结点为空 就得找下一个桶  怎么找？根据当前指向的数据算出桶号  再把桶号+1 一直往后面找 直到找完整个容器或找到  如果没有下一个就返回空





取内嵌类型时加一个typename





