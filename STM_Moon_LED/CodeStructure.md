# IDEA FOR MOONBOARD LED PROJECT



- **BLUE BUTTON INTERRUPT** I press the blue button and activate the function for catching the keypad number
- **keypad_GetNumber** number gets saved
  - polling mode, wait for a key to be pressed, store digit in an array, transform array in number, return number
- **problem_search** STM searches in file JSON corresponding problem, extract useful data
- **problem_transform** transform the problem just defined in a vector that can be sent to LED
  - generate a vector of 200*24 (200 = led number; 24=bits for each led, 888 RGB)
  - define in the vector light colors in specific places, 0 and 1 for correct colour (green starting holds, white middle holds, red last hold)
- **LED_lightProblem** STM generates the correct PWM signal depending on the array that is received





## TO DO

- android app for filtering and visualizing the problems
- script that download and updates list of problem in JSON format
- script that adds a number to the JSON problem file
- script for the LED itnerfacing
- script for the display



## LED

![image-20210416214503904](C:\Users\massi\AppData\Roaming\Typora\typora-user-images\image-20210416214503904.png)







while(problem[i] != ":"){

​    i+=1;

  }

  i+=1;

  while(i <= sizeof(problem)){



​    tmp = problem[i];



​    if(isalpha(tmp)){

​      prob_col[pos] = tmp;

​    }else if(isdigit(tmp)){

​      prob_row[pos] = tmp;

​      pos+=1;

​    }





​    i+=1;

  }



  for(int k=0; k<sizeof(problem); k++){

​    printf("%c", prob_col[k]);

​    printf("%d ", prob_row[k]);

  }