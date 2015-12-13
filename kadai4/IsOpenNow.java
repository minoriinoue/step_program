package com.company;
import java.util.*;
import java.io.*;
import java.lang.String;

/**
 * Created by Minori on 6/20/15.
 */
public class IsOpenNow {

    public static void fileOpen(int[] date){
        try{
            String curDir = System.getProperty("user.dir");
            System.out.println(curDir);
            File file = new File("businesshour.txt");
            System.out.println(file.getAbsolutePath());

            BufferedReader br = new BufferedReader(new FileReader(file));
            Scanner scan = new Scanner(System.in);
            int counter = 1;
            boolean searching = true;
            do {
                String str = br.readLine();
                if(str == null){
                    System.out.println("Reached the end of shop list. No shop matches your criteria right now...");
                    break;
                }

                if(!str.equals("--")){
                    String day = str.substring(0, 0+4); /*get mon/tue/wed/thu/fri...*/
                    if(daymatch(day, date[0])) {
                        if (judge(date, str)) {
                            System.out.println("#" + (int)(counter/7) + " shop is open.");
                            System.out.println("Do you want to try another shop? yes: 1 no: 0");
                            try {
                                int answer = scan.nextInt();
                                if (answer == 0) searching = false;

                            } catch (InputMismatchException e) {
                                System.out.println("Different type input" + e);
                            }
                        }else{
                            System.out.println("#" + (int)(counter/7) + " shop is closed.");
                        }
                    }
                    counter++;
                }
            }while(searching);

            br.close();
        }catch(FileNotFoundException e){
            System.out.println(e);
        }catch(IOException e){
            System.out.println(e);
        }

    }

    public static boolean daymatch(String scannedDay, int currentday){
        String hold = null;
        switch(currentday){
	case 1:
	    hold = "sun";
	    break;
	case 2:
	    hold = "mon";
	    break;
	case 3:
	    hold = "tue";
	    break;
	case 4:
	    hold = "wed";
	    break;
	case 5:
	    hold = "thu";
	    break;
	case 6:
	    hold = "fri";
	    break;
	case 7:
	    hold = "sat";
	    break;
	default:
	    break;
        }
        if(scannedDay.equals(hold)){
            return true;
        }else{
            return false;
        }
    }

    public static int[] getDate(){
        Calendar calendar = Calendar.getInstance();
        int[] dayAndTime = new int[2];
        dayAndTime[0] = calendar.get(Calendar.DAY_OF_WEEK);
        dayAndTime[1] = calendar.get(Calendar.HOUR_OF_DAY);
        return dayAndTime;
    }

    /*text file -- time-time,time-time,time-time... time: 00*/
    public static int[][] checkBusinessHour(String str){
        int numofperiod = ((str.length() - 4 + 1)/ 6); /*(-4: mon_) (+1: no "," at last) (/6:00-00,)*/
        int[][] businesshour = new int[numofperiod][2];
        for(int i = 0, pointer = 4; i < numofperiod ; i++, pointer += 6){ /*pointer = 5: exempt mon_ */
            businesshour[i][0] = Integer.parseInt(str.substring(pointer, pointer+2)); /*(+2: 00-)*/
            businesshour[i][1] = Integer.parseInt(str.substring(pointer+3, (pointer+3)+2));/*(+3: 00-0), (+2: (00-0)+0,)*/
        }
        return businesshour;
    }

    public static boolean judge(int date[], String str){
        int numofperiod = (str.length() - 4 + 1) / 6;
        int[][] businesshour = new int[numofperiod][2];
        businesshour = checkBusinessHour(str);

        boolean isOpen = false;
        for(int i = 0; i < numofperiod; i++){
            if(businesshour[i][0]== 0 && businesshour[i][1] == 0){
                isOpen = false;
            } else if(businesshour[i][0]> businesshour[i][1]){
                if(businesshour[i][0] <= date[1] || businesshour[i][1] > date[1]){
                    isOpen = true;
                }
            }else{
                if(businesshour[i][0] <= date[1] && date[1] < businesshour[i][1]) {
                    isOpen = true;
                }
            }
        }

        return isOpen;

    }

}