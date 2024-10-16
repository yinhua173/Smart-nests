#include "fingerdata.h"
#include <iostream>
#include <vector>
#include "datadata.h"
// 函数声明

void shell_sort(uint8_t arr[], uint8_t len);
uint8_t zhiwen_data[]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32}; // 存储剩余指纹数据 
/*
希尔排序数组用来存储剩余未被使用的指纹
被删除的指纹数据会被添加到数组中，然后进行排序
*/
void shell_sort_task(void *parameter){
    // int arr[] = { 1,2,3,4,5,6,7,8,9,10 };
    uint8_t len = sizeof(zhiwen_data) / sizeof(zhiwen_data[0]);  // 计算数组长度
    shell_sort(zhiwen_data, len);  // 调用希尔排序函数
		Serial.printf("数组希尔排序完成\n");
    // // 打印排序后的数组
    // for (int i = 0; i < len; i++) {
    //     printf("%d ", zhiwen_data[i]);
    // }
    vTaskDelete(NULL); //删除函数创建的空闲任务
}

// 希尔排序函数
void shell_sort(uint8_t arr[], uint8_t len) {
    // 计算初始间隔
    for (int gap = len / 2; gap > 0; gap /= 2) {
        // 对每个间隔进行插入排序
        for (int i = gap; i < len; i++) {
            int temp = arr[i];  // 当前待插入的元素
            int j = i;
            // 移动大于temp的元素
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;  // 插入元素到正确位置
        }
    }
}
// 创建一个空的整数向量,用于存储已录入的指纹数据
std::vector<int> myVector;
/*
向量用来存储已录入的指纹数据，指纹录入成功后，将指纹数据添加到向量中
*/
void vector_task(void *parameter){
    
    // // 添加元素到向量中
    // myVector.push_back(3);
    // myVector.push_back(7);
    // myVector.push_back(11);
    // myVector.push_back(5);

    // 访问向量中的元素并输出
    // std::cout << "Elements in the vector: ";
    // for (int element : myVector) {
    //     std::cout << element << " ";
    // }
    // std::cout << std::endl;

    // // 访问向量中的第一个元素并输出
    // std::cout << "First element: " << myVector[0] << std::endl;

    // // 访问向量中的第二个元素并输出
    // std::cout << "Second element: " << myVector.at(1) << std::endl;

    // // 获取向量的大小并输出
    // std::cout << "Size of the vector: " << myVector.size() << std::endl;

    // // 删除向量中的第三个元素
    // myVector.erase(myVector.begin() + 2);

    // // 输出删除元素后的向量
    // std::cout << "Elements in the vector after erasing: ";
    // for (int element : myVector) {
    //     std::cout << element << " ";
    // }
    // std::cout << std::endl;

    // // 清空向量并输出
    // myVector.clear();
    // std::cout << "Size of the vector after clearing: " << myVector.size() << std::endl;
}
/*
用于给向量添加数据
输入int data
*/
void vector_to_add(int data) {
  myVector.push_back(data);
}
/*
用于给向量删除数据
输入int data
*/
void vector_to_delete(int data) {
  myVector.erase(myVector.begin() + data - 1);
}
/*
用于输出向量空间大小
return myVector.size();
*/
uint8_t vector_out_size() {
	return myVector.size()/2;
}
/*
输出向量元素
*/
void vector_out_num(){
  for (int element : myVector) {
        std::cout << element << " ";
    }
}
/*
比较整数向量与数组1到30之间的元素是否相等，把相等的元素移出数组
*/
void laser_task(){
	for (int element : myVector){
		for (int i = 0; i < 30; i++){
			if (element == zhiwen_data[i]){
				zhiwen_data[i] = 0;
			}
		}
  }
}
/*
将数字移进数组
*/
void laser_to_add(int data){
	for (int i = 0; i < 30; i++){
		if (zhiwen_data[i] == 0){
			zhiwen_data[i] = data;
			break;
		}
	}
}
/*
输出数组第一个不为零的数
*/
uint8_t array_out_first(){
	for (int i = 0; i < 30; i++){
		if (zhiwen_data[i] != 0){
			Serial.printf("数组第一个不为零的数：%d\n", zhiwen_data[i]);
			return zhiwen_data[i];
			break;
		}
	}
  return 0;
}
