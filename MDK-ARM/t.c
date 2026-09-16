如果按键已经锁定
    如果当前PB1是高电平
        如果这是第一次发现高电平
            记录当前时间
        否则
            如果已经保持高电平50ms
                解锁
    否则
        取消松开计时
		
uint8_t find1=0;
uint8_t time=0;
if(key_press==1)
{ 
	if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1) == GPIO_PIN_SET)
	{
		if (find1==0)
		{
			time=HAL_GetTick();
			find1=1;
		}
		else
		{
			if(HAL_GetTick()-time>=50)
			{
				key_press=0;
				find1=0;
			}
		}
	}
	else
	{
		find1=0;
	}
}