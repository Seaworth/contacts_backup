# 通信录备份
**要求：**
- 语言C++
- 编程环境Ubuntu
- 客户端的通信录备份到服务器
- 数据库使用sqlite3<Br/>

**步骤：**<Br/>
1. create_table.cpp
   - 在服务器端创建数据库test.db
   - 添加表CONTACTS，创建字段ID,NAME,TEL
2. select_table.cpp
   - 显示数据库中的内容
3. server.cpp
   - 打开数据库test.db，读取已有的联系人个数
   - 通过socket接收客户端发来的数据
   - 打开数据库test.db，将数据保存到数据库中
   - 关闭数据库
4. client.cpp
   - 通过socket发送数据到服务器<Br/>

**用法：**<Br/>
- 先创建数据库
```
g++ -o create_table create_table.cpp -l sqlite3
./create_table
```
- 再运行服务器
```
g++ -o server server.cpp -l sqlite3
./server
```
- 再运行客户端
```
g++ -o client client.cpp
./client
```
**运行结果:**
 客户端发送，服务器接收
 
 ![image1](https://github.com/Seaworth/contacts_backup/raw/master/result/socket.png)
 
 查看数据库
 
 ![image2](https://github.com/Seaworth/contacts_backup/raw/master/result/sqlite.PNG)
