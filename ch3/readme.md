# raw socket 发送数据
发送数据的时候，需要指定从哪个网络接口发出去

目前我们的raw socket均以 ETH_P_ALL 为参数，监听链路层上所有的报文，有没有办法让raw socket专注于具有某一种特征的报文呢？
