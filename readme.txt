readme.txt
 Created on: 2019��8��8��
     Author: xiong

     
     8.8     22��00���Լ̵���ģ��  22:56����ͨ��
      if(uart_flag&0x01){         //uart0�ж�
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
    8.9     1:00 �ٶȴ�����     3:13 �ٶȹ�ʽ������Ҫ����    
    ע��   ����ʱ����Ҫע��
    8.9     5��30  ���⴫����   uart2����ʧ�ܣ� ����uart4: �ɹ�         7��31 ����ͨ��  ���غ�����ֵ
    
 