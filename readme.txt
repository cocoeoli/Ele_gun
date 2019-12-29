readme.txt
 Created on: 2019年8月8日
     Author: xiong

     
     8.8     22：00测试继电器模块  22:56测试通过
      if(uart_flag&0x01){         //uart0中断
                    if( ce1 ){
                        Charging( 1 );
                        ce1 = 0;
                    }else{
                        Charging( 0 );
                        ce1 = 1;
                    }
                    Emission();
                    uart_flag = 0;
                }
    8.9     1:00 速度传感器     3:13 速度公式计算需要较正    
    注：   发挥时间需要注意
    8.9     5：30  激光传感器   uart2测试失败， 改用uart4: 成功         7：31 测试通过  返回毫米数值
    
 