- c库需要导出的函数

```
int32_t crpc_call(char* data, int32_t length) {
    return 0;
}

```

- 在proto文件中添加一下作为通信协议

```

message CRPCProtocol {
  enum ErrorCode {
    Unknown = 0;
    OK = 1;
    Fail = 2;
  }

  string method = 1; // 方法
  uint64 funHandle = 2; // 回调函数
  bytes request = 3; // 请求
  bytes response = 4; // 回复

  ErrorCode code = 10;
  string msg = 11;
  
  message Inner {
    string method = 1;
    uint64 funHandle = 2;
  }
  Inner inner = 100; // 内部
}

```