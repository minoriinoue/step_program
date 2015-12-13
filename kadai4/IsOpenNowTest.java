package com.company;

import org.junit.Test;
import java.util.Calendar;

import static org.junit.Assert.*;

/**
 * Created by Minori on 6/20/15.
 */
public class IsOpenNowTest {

    @Test
	public void testJudge() throws Exception {

        /*ordinary case with one time period*/
        int[] date = { 1, 20 }; /*date and current time*/
        assertTrue(IsOpenNow.judge(date, "mon 08-22"));

	/*ordinary case with several time periods*/
        assertTrue(IsOpenNow.judge(date, "mon 10-14,16-22"));
        assertFalse(IsOpenNow.judge(date, "mon 10-12"));

        /*threshold case*/
        assertFalse(IsOpenNow.judge(date, "mon 10-20"));
        assertTrue(IsOpenNow.judge(date, "mon 20-22"));
        assertTrue(IsOpenNow.judge(date, "mon 00-23"));

        /*case when closed all day*/
        assertFalse(IsOpenNow.judge(date, "mon 00-00"));

        /*special case when the business hour crosses midnight*/
        int[] date1 = { 1, 3 };
        assertTrue(IsOpenNow.judge(date1, "mon 21-04"));
        assertFalse(IsOpenNow.judge(date1, "mon 21-03"));

        int[] date2 = { 1, 22 };
        assertTrue(IsOpenNow.judge(date2, "mon 21-04"));
        assertFalse(IsOpenNow.judge(date2, "mon 23-03"));

    }


    @Test
	public void testDaymatch() throws Exception {
        assertTrue(IsOpenNow.daymatch("sun", 1));
        assertFalse(IsOpenNow.daymatch("sun", 2));
        assertFalse(IsOpenNow.daymatch("sun", 0));
    }
}