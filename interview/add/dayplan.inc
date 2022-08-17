<?php
/*

 +------------------------------------------------------------------------------+
 | Mamook(R) Software                                                           |
 +------------------------------------------------------------------------------+
 | Copyright (c) 2000-2005 University of Victoria.  All rights reserved.        |
 +------------------------------------------------------------------------------+
 | THE LICENSED WORK IS PROVIDED UNDER THE TERMS OF THE ADAPTIVE PUBLIC LICENSE |
 | ("LICENSE") AS FIRST COMPLETED BY: The University of Victoria. ANY USE,      |
 | PUBLIC DISPLAY, PUBLIC PERFORMANCE, REPRODUCTION OR DISTRIBUTION OF, OR      |
 | PREPARATION OF DERIVATIVE WORKS BASED ON, THE LICENSED WORK CONSTITUTES      |
 | RECIPIENT'S ACCEPTANCE OF THIS LICENSE AND ITS TERMS, WHETHER OR NOT SUCH    |
 | RECIPIENT READS THE TERMS OF THE LICENSE. "LICENSED WORK" AND "RECIPIENT"    |
 | ARE DEFINED IN THE LICENSE. A COPY OF THE LICENSE IS LOCATED IN THE TEXT     |
 | FILE ENTITLED "LICENSE.TXT" ACCOMPANYING THE CONTENTS OF THIS FILE. IF A     |
 | COPY OF THE LICENSE DOES NOT ACCOMPANY THIS FILE, A COPY OF THE LICENSE MAY  |
 | ALSO BE OBTAINED AT THE FOLLOWING WEB SITE: http://www.mamook.net            |  
 |                                                                              |
 | Software distributed under the License is distributed on an "AS IS" basis,   |
 | WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License for |
 | the specific language governing rights and limitations under the License.    | 
 +------------------------------------------------------------------------------+
 | Filename: dayplan.inc                                                        |
 +------------------------------------------------------------------------------+
 | Description: 	Display daily schedule in table form, and corresponding        |
 | submit buttons                                                               |
 +------------------------------------------------------------------------------+

*/

function display($URL, $intdays, $day, $int_date, $schedule, $manual, $control, $control2='', $param = array(), $edit_conflict=0)
{
	global $interview;
	global $interview2;
 	
	assert( $int_date );
	
	assert( $schedule && is_array($schedule) );


	$form_header = ("
		<form method='post' action='$URL'>
		<table border='0' cellpadding='0' cellspacing='0' width='100%'>
	");
	if (sizeof($param) > 1){	
		foreach($param as $name => $value)
		{
		$form_header .= ("<input type='hidden' name='$name' value='$value' />");
		}
	}
	echo($form_header);

	echo("
	<tr>
   	<td colspan='5' align='center' class='row0'>		<input type='hidden' name='intdays' value='$intdays' />
		<input type='hidden' name='day' value='$day' />
		<input type='hidden' name='int_date' value='$int_date' />
		<input type='hidden' name='interview' value='" . packObject(($interview)) . "' />
		<input type='hidden' name='schedule' value='" . packObject(($schedule)) . "' />
	
	<b class='white'>" . formatLongDate($int_date) . "</b>
	</td>
   	</tr>
	");
	
/***		
	create schedule time-table ( type, start time, end time ) 
***/
	echo("<tr><td colspan='5'>");
	if ($manual != 1 && $control2)
	{
		echo("
		<table border='0' width='100%' cellspacing='0' cellpadding='0'><tr><td>
		");
	}
	$add_time_list = displayTimeTable($schedule, $manual, $int_date, 1);
	echo("</td></tr>");
	if ($edit_conflict){
		echo "<tr><td>";
		display_edit_conflict($interview2, $int_date);
		echo "</td></tr>";
	}
/*  	
	according to input mode( manual or calculator ), display corresponding
	submit buttons		
*/
	if( $manual == 1 )
	{
		echo("
		<input type='hidden' name='submit' value='Time Calculator' />
		<tr>
		  <td colspan='5'><hr width='100%' />
			<table border='0' width='100%' class='row1' cellspacing='0' cellpadding='4' class='row1'>
			  <tr>
			    <td align='center' width='50%'><input type='submit' name='time_mode' value='Update and Save changes' /></td>
			    <td align='center'><input type='submit' name='time_mode' value='Close manual mode' /></td>
			  </tr>
		");
		if (!$control2)
		{
			echo("
			</table>");
			//</td>
			//</tr>
			//");
		}

	}
	else //We're not in manual mode.
	{

		if (!$control2)

		/*
		 We've haven't been passed customized buttons, so display these ones.
		*/

		{
			echo("
			<input type='hidden' name='submit' value='Time Calculator' />
			<tr>
			  <td colspan='5'>
			    <br /><table border='0' width='100%' class='row1' cellspacing='0' cellpadding='4'>
			      <tr>
			  	<td align='left' width='33%'><input type='submit' name='time_mode' value='Reset Times' /></td>
			  	<td align='center' width='33%'><input type='submit' name='time_mode' value='Manual mode' /></td>
			  	<td align = 'right'><input type='submit' name='time_mode' value='Remove selected event' /></td>
			      </tr>
			    </table>
			    <br />
			  </td>
			</tr>
		  	");	
			//</table>
			//");
		
		} 
		else //We've been given a $control2
		{
			echo("
			<tr>
			  <td colspan='1'><br /></td>
			</tr>
			<tr>
			  <td align='right' colspan='1'><input type='submit' name='time_mode' value='Remove selected event' /></td>
			</tr>
			</table>
			</td></tr>
			");
	
		}//if (!$control2)


	/***	 add a new event( break, lunch )		***/
	
		echo("
		<tr>
		  <td colspan='5'><hr />
		    <table border='0' cellpadding='2' cellspacing='0' width='100%'>
			<tr class='row0d'>
			  <td colspan='3' align='center'>
				<table border='0' cellspacing='0' cellpadding='2' width='100%' class='row1'>
				  <tr>
				    <td width='20%' align='right' nowrap='nowrap'>Select Type:&nbsp;</td>
				    <td align='left'><select name='add_type' tabindex=1>
					<option value='2'>BREAK</option>
					<option value='3'>LUNCH</option>
					<option value='1'>INTERVIEW</option>
					</select>
				    </td>
				    <td align='right'>&nbsp;</td>
				  </tr>
				  <tr>
				    <td width='20%' align='right' nowrap='nowrap'>Start Time:&nbsp;</td>
				    <td align='left' colspan='2'>
					<select name='add_start' tabindex=2>");
					foreach( $add_time_list as $t )
					{
						echo("<option value='$t'>" . formatTime($t) . "</option>");
					}
		echo("				</select>&nbsp;&nbsp;&nbsp;&nbsp;OR&nbsp;&nbsp;&nbsp;&nbsp;<input type='text' name='add_start_m' size=9 maxlength=8 tabindex=3 />
				    </td>
				  </tr>
				  <tr>
		  		    <td width='20%' align='right' nowrap='nowrap'>Duration:&nbsp;</td>
				    <td align='left'><input type='text' name='add_length' size=4 maxlength=3 tabindex=4 />minutes</td>
			    <td align='right'><input type='submit' name='time_mode' value='Add to schedule' tabindex=5 />&nbsp;</td>
				  </tr>
				</table>
			    </td>
			  </tr>
			</table>
		    <hr /></td>
		</tr>
		<tr>
		  <td colspan='5' width='100%'>
		");
	
		if ($control2)
		{
			echo($control2);
		} 
		echo ($control);
	
/*** 		add customized control buttons		***/
	
	} // (!$manual)

	if ($control2 && $manual)
	{
		//if ($manual)
		//{
			echo ("</table></td></tr></table></form>");
			
			//echo("
			//</table>
			//</tr>
			//</td>
			//</form>
			//</table>
			//");
		//} 
	} else
	{
		echo("
		</td>
		</tr>
		</table>
		</form>
		");
		
	}

	

}


function print_schedule()
{
 	global $schedule;
	
	foreach( $schedule as $i=>$s )
	{
		echo( $i . " => " . $s['TYPE'] . "&nbsp;" . $s['START'] ."&nbsp;". $s['END'] . "<br />" ); 	
	}
	echo("<hr />");
}




/*	
 Add a break or lunch to the daily schedule, also used for change 
 interview time.  *NEW*  This code has been altered, due to a major
 flaw in the code that occurred when the user attempted to add a 
 break or lunch during the middle of an interview, break, or lunch
 (Why are they doing this in the first place)?  Code modified by
 Adam on 2001-02-15.
*/

function add_to_schedule( $add_start, $add_type, $add_length, $add_start_m=null )
{
	global $schedule;

	$add_type = intval($add_type);
	$add_length = intval($add_length);

	/*
	 Two new variables added below, one to detect conflicts, another
	 to make things simpler ($add_end).
	*/

	$conflict_found = false;
	
	if( $add_length <=0 ) 	return;
	
	/* check if manually input start time is specified */
	if(  $add_start_m != null  )
	{
	 	if( ($s = validTime($add_start_m)) == -1 )	 return;
	 	$add_start = add_times( $s, 0 );
	}

	$add_end = add_times($add_start, $add_length);
	
	$insert_in = -1;
		
	for( $i = 0; $i < count($schedule); $i++ )
	{
		if( $schedule[$i]['START'] <= $add_start && $schedule[$i]['END'] > $add_start )
		{
			$conflict_found = true;
			$insert_in = $i;
			break;
		}
		if( $schedule[$i]['START'] > $add_start )
		{
			$insert_in = $i;
			break;
		}
	}
	
if( $insert_in != -1 )
{

	/*
	 We now have the index where we will insert, and we have a variable
	 letting us know if we've run into a conflict ($conflict_found).
	 If we did find a conflict, we must find out what type of conflict
	 it was that we found.
	*/

	if ($conflict_found)
	{

	if ($schedule[$insert_in]['TYPE'] == intval(INTERVIEW))
	{
		$conflict_type = 0;
	} elseif ($schedule[$insert_in]['TYPE'] == intval(INT_BREAK))
	{

	/*
	 At this point we know for a fact that the inserted object
	 starts in the middle of a break or a lunch.
	*/
		if ($schedule[$insert_in]['END'] > $add_end)
		{
		
		/*
		 The object being inserted is contained within
		 the object it's being inserted into.
		*/

			if ($add_type == intval(INT_BREAK))
			{
				$conflict_type = 1;
				if ($add_start == $schedule[$insert_in]['START'])
				{
					$conflict_type = 0;
				}
			} elseif ($add_type == intval(LUNCH))
			{
				$conflict_type = 7;
				if ($add_start == $schedule[$insert_in]['START'])
				{
					$equal_start = true;
				}
				if ($add_end == $schedule[$insert_in]['END'])
				{
					$equal_end = true;
				}
			}
		} //If we're inserting an object contained within a object
			

		elseif ($schedule[$insert_in]['END'] <= $add_end)
		{

		/*
		 The object being inserted ends later than the object
		 it is being inserted into does.
		*/

			if ($add_type == intval(INT_BREAK))
			{
				$conflict_type = 2;
			} elseif ($add_type == intval(LUNCH))
			{
				$conflict_type = 9;
				if ($add_start == $schedule[$insert_in]['START'])
				{
					$equal_start = true;
				}
			}
		}
	} // The object we're inserting into isn't a break.
	else 
	{

	// The object we're inserting into is a lunch.
	
		if ($add_end < $schedule[$insert_in]['END'])
		{
			$conflict_type = 4;
		} elseif ($add_end > $schedule[$insert_in]['END'])
		{
			$conflict_type = 5;
		}
	} // End of deciding what type of object we're inserting into.

	} // End of determing conflict_type.

	switch($conflict_type)
	{

	case "1":	
	
	// Inserting a break into the middle of another break.  Do nothing.
	
		break;
	
	case "2":
	
	// Inserting a break into a break and they overlap.  Create one long
	// break.
		
		$schedule[$insert_in]["END"] = $add_end;
		for ($i = ($insert_in + 1); $i < sizeof($schedule); $i++)
		{
			if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
			{
				$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
				$schedule[$i]["START"] = $schedule[$i-1]["END"];
			}
		}
	
		break;

	case "4":
	
	// Inserting a break into a lunch.  Do nothing.
	
		break;

	case "5":
	
	// Inserting a break into a lunch and they overlap.  Shorten break.
	
		$add_start = $schedule[$insert_in]["END"];
		$add_length = cal_length($add_start, $add_end);
		$insert_in += 1;
		for( $i = count($schedule); $i > $insert_in; $i--)
		{
			$schedule[$i] = $schedule[$i-1];
		}
		$schedule[$insert_in] = array("START"=>$add_start, "END"=>add_times($add_start, $add_length), "TYPE"=>intval( $add_type ) );

		$size = count( $schedule );
		
		for( $i=($insert_in + 1); $i < $size; $i++ )
		{
			if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
			{
				$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
				$schedule[$i]["START"] = $schedule[$i-1]["END"];
			}
		}
	
		break;
	
	case "7":
	
	// Inserting a lunch into the middle of the break. Change middle of the
	// break to be a lunch.

		if ($equal_start && $equal_end)
		{
			$schedule[$insert_in]['TYPE'] = intval($add_type);
		} elseif ($equal_start)
		{
			$newbreak_start = $add_end;
			$newbreak_end = $schedule[$insert_in]["END"];
			$newbreak_type = intval(INT_BREAK);
			$schedule[$insert_in]['TYPE'] = intval(LUNCH);
			$schedule[$insert_in]['END'] = $add_end;
			
			// Insert the break back in now.

			for($i = count($schedule); $i > $insert_in; $i--)
			{
				$schedule[$i] = $schedule[$i-1];
			}

			$schedule[$insert_in + 1] = array("START"=>$newbreak_start, "END"=>$newbreak_end, "TYPE"=>intval($newbreak_type ) );

			$size = count( $schedule );
		
			for( $i=($insert_in + 1); $i < $size; $i++ )
			{
				if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
				{
					$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
					$schedule[$i]["START"] = $schedule[$i-1]["END"];
				}
			}
		} elseif ($equal_end)
		{
			$schedule[$insert_in]['END'] = $add_start;
			
			// Now insert the lunch in.
	
			for($i = count($schedule); $i > $insert_in; $i--)
			{
				$schedule[$i] = $schedule[$i-1];
			}

			$schedule[$insert_in + 1] = array("START"=>$add_start, "END"=>$add_end, "TYPE"=>intval($add_type ) );

			$size = count( $schedule );
		
			for( $i=($insert_in + 1); $i < $size; $i++ )
			{
				if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
				{
					$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
					$schedule[$i]["START"] = $schedule[$i-1]["END"];
				}
			}
		} else
		{

			$newbreak_start = $add_end;
			$newbreak_end = $schedule[$insert_in]["END"];
			$newbreak_type = intval(INT_BREAK);
			$schedule[$insert_in]["END"] = $add_start;
			$insertnew_break_in = $insert_in + 1;
	
			// Insert the lunch in.
	
			for( $i = count($schedule); $i > $insert_in; $i--)
			{
				$schedule[$i] = $schedule[$i-1];
			}
			$schedule[$insert_in + 1] = array("START"=>$add_start, "END"=>add_times($add_start, $add_length), "TYPE"=>intval( $add_type ) );
	
			$size = count( $schedule );
			
			for( $i=($insert_in + 1); $i < $size; $i++ )
			{
				if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
				{
					$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
					$schedule[$i]["START"] = $schedule[$i-1]["END"];
				}
			}
	
			// Insert the last bit of the break.
		
	
			for( $i = count($schedule); $i > $insertnew_break_in; $i--)
			{
				$schedule[$i] = $schedule[$i-1];
			}
	
			$schedule[$insertnew_break_in + 1] = array("START"=>$newbreak_start, "END"=>$newbreak_end, "TYPE"=>intval($newbreak_type ) );
	
			$size = count( $schedule );
			
			for( $i=($insertnew_break_in + 1); $i < $size; $i++ )
			{
				if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
				{
					$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
					$schedule[$i]["START"] = $schedule[$i-1]["END"];
				}
			}
		}
		break;

	case "9":

	// We're inserting a lunch into a break and the lunch overlaps the
	// break.  Shorten the break, and add in the lunch.
	
		if ($equal_start) 
		{
			$schedule[$insert_in]["TYPE"] = $add_type;
			$schedule[$insert_in]["END"] = $add_end;
			$size = count( $schedule );
		
			for( $i=($insert_in + 1); $i < $size; $i++ )
			{
				if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
				{
					$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
					$schedule[$i]["START"] = $schedule[$i-1]["END"];
				}
			}
		} else
		{			
	
			$schedule[$insert_in]["END"] = $add_start;
			for( $i = count($schedule); $i > ($insert_in); $i--)
			{
				$schedule[$i] = $schedule[$i-1];
			}
			$schedule[$insert_in + 1] = array("START"=>$add_start, "END"=>add_times($add_start, $add_length), "TYPE"=>intval( $add_type ) );
	
			$size = count( $schedule );
			
			for( $i=($insert_in + 2); $i < $size; $i++ )
			{
				if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
				{
					$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
					$schedule[$i]["START"] = $schedule[$i-1]["END"];
				}
			}
		}	
		break;
	
	case "0":
	default:
		
	// Insert the break or lunch into schedule, and adjust overlapping 
	// time slots.		
	
		for( $i = count($schedule); $i > $insert_in; $i--)
		{
			$schedule[$i] = $schedule[$i-1];
		}
		$schedule[$insert_in] = array("START"=>$add_start, "END"=>add_times($add_start, $add_length), "TYPE"=>intval( $add_type ) );

		$size = count( $schedule );
		
		for( $i=($insert_in + 1); $i < $size; $i++ )
		{
			if($schedule[$i]["START"] < $schedule[$i-1]["END"]) 
			{
				$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
				$schedule[$i]["START"] = $schedule[$i-1]["END"];
			}
		}
	break;
	} // Switch statement.

} // If statement.
else
{
	/***  add the break or lunch to the end of all 	***/
	$schedule[count($schedule)] = array("START"=>$add_start, "END"=>add_times($add_start, $add_length), "TYPE"=>$add_type );
} // Else.

/*
 Now merge any adjacent breaks together, and we're done.
*/

	$temp_schedule = $schedule;
	for( $i = 0; $i < sizeof($temp_schedule); $i++ )
	{
		if (($temp_schedule[($i-1)]['TYPE'] == intval(INT_BREAK)) && ($temp_schedule[$i]['TYPE'] == intval(INT_BREAK)))
		{
			$temp_schedule[$i]['START'] = $temp_schedule[($i-1)]['START'];
			unset($temp_schedule[($i-1)]);
			$schedule = restoreIndex($temp_schedule);
			$temp_schedule = $schedule;
		}
	}
	
	$schedule = restoreIndex($temp_schedule);

} // Function.

/*
 Name: update_manual_schedule()
 After users manually input schedule, validate schedule, if not valid, make no change to previous schedule.
*/

function update_manual_schedule()
{
	global $m_schedule;
	global $schedule;
	global $interview;

	if (isSequential($interview->interview_type))
	{
	
		for ($i = 0; $i < count($m_schedule); $i++)
		{
			if ($m_schedule[$i][3] == INTERVIEW && trim($m_schedule[$i][2]))
			{
				if ($int_lengths)
				{
					if ($int_lengths != $m_schedule[$i][2])
					{
						error("All interview lengths must be the same for sequential interviews.");
						return;
					}
				}
				else
				{
					$int_lengths = $m_schedule[$i][2];
				}
			}
		}
	}

	for( $i=0; $i<count($m_schedule); $i++ )
	{
		if( ($s = validTime($m_schedule[$i][1])) == -1 )
		{
			error("Invalid time format {$m_schedule[$i][1]}");
			return;
		}
		else { $s = add_times( $s, 0 ); }

	        if( trim($m_schedule[$i][2]) )
	        {
        		if ( ($l =intval( $m_schedule[$i][2] )) > 0 ){}
          		else	
          		{	
          			error("Invalid duration {$m_schedule[$i][2]}, which must be greater than 0. If you want to remove a break or lunch, leave its duration field empty."); 	
          			return;	
          		}
        	}
     		elseif( $m_schedule[$i][3] == INTERVIEW )
     		{       
			/* interview can not be removed by leaving duration field empty */
       	   		error("Interview duration can not be empty."); 	
          		return;
      		}	
     		else 
		{	
			//duration is empty, so remove corresponding event ( break, lunch )
     		
     			$temp_schedule[] = "EMPTY";
     			continue;
     		}	

		/* 		if the slot's start time wasn't changed, adjust it  according to the structure of original schedule 	*/
		
		if( $s == $schedule[$i]['START'] && isset( $schedule[$i-1] ) && ( $schedule[$i]['START'] == $schedule[$i-1]['END'] ) )
		{
		        $j = $i -1;

			while( $temp_schedule[$j] == "EMPTY"  && ( isset($schedule[$j-1]) && $schedule[$j]['START'] == $schedule[$j-1]['END'] ) )
			{
				$j--;							
			}
			if( $temp_schedule[$j] == "EMPTY" ) { $s = $schedule[$j]['START']; }
			else {	$s = $temp_schedule[$j]['END']; }
		}
		$temp_schedule[] = array("START"=>$s, "END"=>add_times( $s, $l ), "TYPE"=>$m_schedule[$i][3]);	
 	}
	 	
 	/*	remove empty slots and restore index	*/
	for( $i = 0; $i < count($temp_schedule); $i++ )
	{
		if( $temp_schedule[$i] == "EMPTY" )
		{
			unset($temp_schedule[$i]);
		}
	}
	
	$schedule = restoreIndex($temp_schedule);
	
    /* adjust schedule to ensure no overlapping slots */

    for( $i=0; $i < count($schedule); $i++ )
	{
			
		if($schedule[$i]["START"] < $schedule[$i-1]["END"])
		{
			$schedule[$i]["END"] = add_times( $schedule[$i-1]["END"], cal_length($schedule[$i]["START"], $schedule[$i]["END"]) );
			$schedule[$i]["START"] = $schedule[$i-1]["END"];
		}
	}
	

}

/*	 subtract given number of minutes from given time, return the new time	*/
function subtract_times($time, $int_length)
{
   $splittime = explode(":", $time);

   $min = intval($splittime[1]);
   $hour = intval($splittime[0]);
   $int_length = intval($int_length);

   $min = $min-$int_length;

   while ($min < 60)
   {
      $hour--;
      $min = $min + 60;
   }
   while( $hour < 0 )
   {
      $hour = $hour + 24;
   }
   $time = date( "H:i", mktime( $hour, $min ) );
   $time = $time . ":00";
   return $time;
}

function remove( $index )
{
	global $schedule;

/*
 New coded added below.  This code gives the user the ability to also remove
 interview slots from the schedule.  Should they choose to do so, breaks must
 be accounted for.  Actions:
 - Interview is on an edge and has no breaks adjacent to it, the interview is 
   simply removed.
 - Interview is on an edge and there is a break adjacent to it, the break is al   so removed.  
 - If the interview is adjacent to only one break and not on an edge, that 
   break is lengthened to account for the removed interview.
 - If the interview is sandwiched between two breaks, one break is 
   removed along with the interview, and the other break is lengthened
 - If there are no adjacent breaks and interview is not on an edge, 
   but breaks exist in the schedule, the interview is replaced with a break.
  *Code added by Adam on 2001-01-31.*
*/
 	if ($index < 0)
	{
		return;
	}

	/*
	 If the event being removed is in the first or last slot, then 
	 we don't need to worry about adding a break in the place that 
	 we remove the event from.
	*/

	$onanedge = false;		//Determines if removed slot is on an 
					//edge
	$breaks = false;		//Determines if we replace with a break
	$breakbefore = false;		//Determines if there is a break before
	$breakafter = false;		//Determines if there is a break after
	$breakbeforeedge = false;	//Determines if the break before is on
	 				//an edge
	$breakafteredge = false;	//Determines if the break after is on
					//an edge
	
	
	if ($index == 0)
	{
		$onanedge = true;
		if ($schedule[($index + 1)]['TYPE'] == INT_BREAK)
		{
			$breakafter = true;
		}
	} elseif ($index == (sizeof($schedule) - 1))
	{
		$onanedge = true;
		if ($schedule[($index - 1)]['TYPE'] == INT_BREAK)
		{
			$breakbefore = true;
		}
	}
	
	/*		
	 First thing we need to do here is check and see if the schedule
	 has breaks in it or not.  If it does, then set $breaks to true
	 otherwise don't.
	*/


	if (!$onanedge)
	{
		for ($i = 0; $i < sizeof($schedule); $i++)
		{
			if ($schedule[$i]['TYPE'] == INT_BREAK)
			{
				$breaks = true;
				break;
			}
		}
	
	/*
	 If there are breaks, then we need to see if there is one before
	 or after the slot that we are removing.
	*/

		if ($schedule[($index - 1)]['TYPE'] == INT_BREAK)
		{
			$breakbefore = true;
			if (($index - 1) == 0)
			{
				$breakbeforeedge = true;
			}
		}
		if ($schedule[($index + 1)]['TYPE'] == INT_BREAK)
		{
			$breakafter = true;
			if (($index + 1) == (sizeof($schedule) - 1))
			{
				$breakafteredge = true;
			}
		}
	} // if (!$onanedge)

	/*
	 Now we know if the event being removed is in the first or last
	 slot ($onanedge is true if it was), if there are breaks in the
	 schedule ($breaks is true if there are), and if there was
	 a break before or after the removed event ($breakbefore and
	 $breakafter will be true if there is).  Also, now we know 
	 if the interview has an adjacent break that is on an edge.
	*/


	/*
	 If the removed event was an interview, we will need to consider 
	 the possibility of adding a break in the place of where the 
	 interview used to be.
	*/

	if (($schedule[$index]['TYPE'] == INTERVIEW) && !$onanedge)
	{

		/*
		 The slot being removed is not on an edge, but is 
		 an interview.
		*/

		if ($breaks)
		{

		/*
		 There are breaks in the schedule, so we need to add 
		 another one where the interview removed used to be.
		*/
	
		if ($breakbefore && $breakafter)
		{
			if ($breakbeforeedge || $breakafteredge)
			{
				unset($schedule[($index - 1)]);
				unset($schedule[($index)]);
				unset($schedule[($index + 1)]);
				$schedule = restoreIndex( $schedule );
			} else 
			{
				
				
			/*
			 In this case, we need to remove the selected
			 event, as well as the break afterwards, and
			 make the break before end at the end of the 
			 break afterwards.
			*/
			
			$newbreakend = $schedule[($index + 1)]['END'];
			$schedule[($index - 1)]['END'] = $newbreakend;

			/*
			 Now the break before the interview has become one
			 big break.  All that is left to do now is remove
			 the interview and the break afterwards from the
			 schedule.
			*/

			unset($schedule[($index + 1)]);
			unset($schedule[($index)]);
			$schedule = restoreIndex( $schedule );

			}
			
		} elseif ($breakbefore)
		{
			if ($breakbeforeedge)
			{
				unset($schedule[($index - 1)]);
				unset($schedule[($index)]);
				$schedule = restoreIndex( $schedule );
			} else
			{
			
			/*
			 There is only a break before the removed slot.
			*/

			$newbreakend = $schedule[($index)]['END'];
			$schedule[($index - 1)]['END'] = $newbreakend;

			unset($schedule[($index)]);
			$schedule = restoreIndex( $schedule );
			
			}
			
		} elseif ($breakafter)
		{
			if ($breakafteredge)
			{
				unset($schedule[($index + 1)]);
				unset($schedule[($index)]);
				$schedule = restoreIndex( $schedule );
			} else
			{
		
			/*
			 There is only a break after the removed slot.
			*/
			
			$newbreakstart = $schedule[($index)]['START'];
			$schedule[($index + 1)]['START'] = $newbreakstart;
			
			unset($schedule[($index)]);
			$schedule = restoreIndex( $schedule );

			}

		} else // There are no breaks adjacent to this event
		{
		
			$schedule[$index]['TYPE'] = INT_BREAK;
			
			}
		} else //There are no breaks in the schedule
		{

			unset($schedule[($index)]);
			$schedule = restoreIndex( $schedule );
		}
	} elseif ($schedule[$index]['TYPE'] == INTERVIEW)
	{
	
	/*
	 The slot being removed is an interview and is either in the
	 first or last slot in the schedule.
	*/
	
		if ($onanedge && $breakafter)
		{
			unset($schedule[($index + 1)]);
		} elseif ($onanedge && $breakbefore)
		{
			//echo("WUDAN TEMPLE");
			unset($schedule[($index - 1)]);
		}

		unset($schedule[$index]);
		$schedule = restoreIndex( $schedule );
	} else
	{
		
	$prev = $schedule[$index];
	
	$length = cal_length( $schedule[$index]['START'], $schedule[$index]['END']);

	for( $j = ($index + 1); $j < count($schedule); $j++ )
	{
		if( $schedule[$j]['START'] == $prev['END'] )
		{
			$prev = $schedule[$j];
			$schedule[$j]['START'] = subtract_times($schedule[$j]['START'], $length );
			$schedule[$j]['END'] = subtract_times($schedule[$j]['END'], $length );
		}
		else { break; }
	}
			
	unset($schedule[$index]);
	
	$schedule = restoreIndex( $schedule );

	}
	
}

function displayTimeTable( $schedule, $manual, $int_date=null, $showDeleteBox=null)
{
	$j = 1;
	global $interview;
	global $job_id;
	global $URL;
		

	
	echo("<table border='0' cellpadding='5' cellspacing='0' width='100%'>");
	
	for($i = 0; $i < count($schedule); $i++)
	{
		if( $schedule[$i]['TYPE'] == INTERVIEW ) {	

			$add_time_list[] = $schedule[$i]["START"];
			if( $i ==  (count($schedule)-1) ) 
			{
				$add_time_list[] = $schedule[$i]["END"];	
			}
			$header = "INTERVIEW&nbsp;" . ($j++);
			if ($int_date && (!isSwitching($interview->interview_type)))
			{
				$sql =  "SELECT student_number 
					FROM interview_time
					WHERE int_time='" . $schedule[$i]["START"] . "'
					AND int_date='" . $int_date . "'
					AND end_time='" . $schedule[$i]["END"] . "'
					AND job_id='" . $job_id . "'
					AND student_number IS NOT NULL
					AND sequential='0'
					";
				$tempresult = $GLOBALS['dbh']->Execute($sql);
				if ($temprow = ($tempresult->FetchRow()))
				{
					$studentSignedup = 1;
				}
				$tempresult->Close();
			} elseif ($int_date && (isSwitching($interview->interview_type)))
			{
				$tempresult = $GLOBALS['dbh']->Execute("
					SELECT student_number 
					FROM interview_time
					WHERE int_time='" . $schedule[$i]["START"] . "'
					AND int_date='" . $int_date . "'
					AND job_id='" . $job_id . "'
					AND student_number IS NOT NULL
					");
				if ($temprow = ($tempresult->FetchRow()))
				{
					$studentSignedup = 1;
				}
				$tempresult->Close();
			}

/*
 The below code has been changed to make it possible to delete interview
 times.  $tail previously was assigned "</td><td>&nbsp;</td></tr>".
*/

			$tail = ("
				</td>
				<td align='center'>
				");

				
			if ( $showDeleteBox == 1 && !($studentSignedup)) 
			{
				$tail .= ("
				<input type='radio' name='remove_list[]' value='$i' />
				");

			} elseif ( $studentSignedup )
			{
				$tail .= ("
				<img src='misc/images/student.gif' alt='' />
				");
			} else
			{
				$tail .= ("&nbsp;");
			}
			$tail .= ("</td></tr>");
			
		

/*
 End of new code.
*/

		}//TYPE was interview

		elseif( $schedule[$i]['TYPE'] == INT_BREAK )
		{	
			$header = "<b>BREAK</b>";
			$tail = "</td><td align='center'>" . (( $showDeleteBox == 1 )? "<input type='radio' name='remove_list[]' value='$i' />" : "&nbsp;" ) . "</td></tr>";
		}
		elseif( $schedule[$i]['TYPE'] == LUNCH )
		{
			$header = "<b>LUNCH</b>";
			$tail = "</td><td align='center'>" . (( $showDeleteBox == 1 )? "<input type='radio' name='remove_list[]' value='$i' />" : "&nbsp;" ) . "</td></tr>";
		}
		echo("
		<tr class='" . (($schedule[$i]['TYPE'] == 1)? 'row0d' : 'row1d') . "'>
		");
		echo("
		<td align='center' nowrap='nowrap'>
		$header
		</td>
		");

				
		if( $manual == 1 )
		{
			echo("
			<td align='left'  nowrap='nowrap'>
			<input type='hidden' name='m_schedule[$i][3]' value='{$schedule[$i]['TYPE']}' />
			<input type='text' name='m_schedule[$i][1]' value='" . formatTime($schedule[$i]['START'], false) . "' size='12' maxlength='8' />&nbsp;&nbsp;&nbsp;&nbsp;
			<b>-
			</b>&nbsp;&nbsp;&nbsp;&nbsp;". formatTime($schedule[$i]['END'], false) . "
			</td>
			<td align='left' nowrap='nowrap'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<input type='text' name='m_schedule[$i][2]' value='" . cal_length($schedule[$i]['START'], $schedule[$i]['END']) . "'size='4' maxlength='3' />&nbsp;minutes
			</td>
			<td>");
			if ($studentSignedup)
			{
				echo("<img src='misc/images/student.gif' alt='' />");
			} else
			{
				echo("&nbsp;");
			}
			echo("
			</td>
			</tr>");

		}
		else
		{
			echo("
			<td align='right'  nowrap='nowrap'>" . formatTime($schedule[$i]['START']) . "
			</td>
			<td align='center' width='10%'>-
			</td>
			<td align='left'   nowrap='nowrap'>" . formatTime($schedule[$i]['END'])
			);
			echo($tail);
		}
		unset($studentSignedup);
	}
	
	echo("</table>");
	
	return $add_time_list;
}

function restoreIndex( $inputArray )
{
	if( count($inputArray) == 0 )
	{
		$inputArray = array();
	}
	else
	{
		assert( empty( $temp ) );
		foreach( $inputArray as $element )
		{
			$temp[] = $element;
		}
		$inputArray = $temp;
	}
	return $inputArray;	
}	

function display_edit_conflict($interview, $int_date=""){

	echo "<br /><b class='black'>Your Changes:</b>";
	for($i=0;$i<count($interview->schedules);$i++){
		if ($int_date == "" || $interview->int_dates[$i]==$int_date){
			echo "<table border='0' width='100%' cellpadding='0' cellspacing='0'>";
			echo "<tr><td class='row0' align='center'><b class='white'>" . FormatLongDate($interview->int_dates[$i]) . "</b></td></tr>";
			echo "<tr><td><table width='100%' border='0' cellpadding='5' cellspacing='0'>";
			for($j=0;$j<count($interview->schedules[$i]);$j++){
				switch($interview->schedules[$i][$j]['TYPE']){
					case 1:
						$type = "INTERVIEW " . ($j + 1);
						$class="row0d";
						break;
					case 2: 
						$type = "BREAK";
						$class="row1d";
						break;
					case 3:
						$type = "LUNCH";
						$class="row1d";
						break;
					default:
						$type = "UNKNOWN";
						$class="row1d";
				}
				echo "<tr>";
				echo "<td class='$class' align='center'>" . $type . "&nbsp;</td>";
				echo "<td class='$class' align='center'>" . FormatTime($interview->schedules[$i][$j]['START'], false) . "&nbsp;</td>";
				echo "<td class='$class' align='center'>-</td>";
				echo "<td class='$class' align='center'>" . FormatTime($interview->schedules[$i][$j]['END'], false) . "&nbsp;</td>";
				echo "</tr>";
			}
			echo "</table></td></tr>";
			echo "</table><br />";
		}
	}

}

?>
