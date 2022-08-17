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
 | Filename: view_interviews_by_date_by_student.inc                             |
 +------------------------------------------------------------------------------+
 | Description: * Original *  filename: view_interviews_by_date.inc *  $date,   |
 | $department must be sent by the script that includes this file. *            |
 +------------------------------------------------------------------------------+

*/

include ("misc/mergesort.inc");

   $order_directive = "";

   $int_date = $date;
	/*SELECT distinct ss.student_number, s.first_name, s.last_name, j.job_id, j.job_code, d.company_name, 
               z.emailed_employer, z.student_visible,
               ip.description, z.interview_medium_id, z.interview_place_id 
	WHERE ss.job_id=j.job_id
	AND j.department_id IN (" . $department_in_str . ")
	AND j.employer_id=d.employer_id
	AND z.job_id=j.job_id
        AND ss.student_number=s.student_number
        AND z.interview_place_id=ip.interview_place_id
	AND it.int_date=\"$date\"
	AND NOT(z.cancelled)
        $order_directive");*/

   $sql = ("
	SELECT distinct j.job_id 
        FROM job_info as j, interview_time as it, students_shortlisted as ss
        WHERE ss.job_id=j.job_id
        AND it.job_id=j.job_id
        AND it.int_date=\"$date\"
        AND j.department_id in (".$department_in_str.")");

   $result = $GLOBALS['dbh']->Execute ($sql);

   if ($result->RecordCount() == 0)
   {
      // No interviews. Period.
      notify("There are no interviews for this date.");
   }
   else      
   {
      // there are interviews in the system

      // Create an URL named $Earl.
      $Earl = "$PHP_SELF&amp;select=view&amp;level1=date&amp;level2=all_interviews_by_student&amp;btnsubmit=View+Results&amp;cal_date=${date}&amp;department_id=${department_id}";

      echo("<h4 align='left'>Interviews on ". $int_date.":</h4>");
                ?>
                  <table cellspacing="0" cellpadding="0" border="1"><tr><td>
                     <table border="0" cellpadding="2">
                        <tr>  
                           <td class="rowgrey" align='center'>&nbsp;<a href="<?php echo $Earl?>&amp;sortby=STUD"><b class="white">Student</b></a>&nbsp;</td>
                           <td class="rowgrey" align='center'>&nbsp;<a href="<?php echo $Earl?>&amp;sortby=COMP"><b class="white">Company</b></a>&nbsp;</td>
			   <td class="rowgrey" align='center'>&nbsp;<a href="<?php echo $Earl?>&amp;sortby=JOBC"><b class="white">Job Code(s)</b></a>&nbsp;</td>
			   <td class="rowgrey" align='center'>&nbsp;<a href="<?php echo $Earl?>&amp;sortby=TIME"><b class="white">Time</b></a>&nbsp;</td>
			   <td class="rowgrey" align='center'>&nbsp;<a href="<?php echo $Earl?>&amp;sortby=LOCN"><b class="white">Location/Phone</b></a>&nbsp;</td>
			   <td class="rowgrey" align='center'>&nbsp;<a href="<?php echo $Earl?>&amp;sortby=TYPE"><b class="white">Type</b></a>&nbsp;</td>
                        </tr>
               <?php

      $listOfStuff = array();
 
      if ($sortby == "STUD" || !isset($sortby))
	$order_directive=" order by s.last_name";

      while ($row = $result->FetchRow())
      {
         $job_id = $row["job_id"];

         $sql = "SELECT distinct j.job_code,ec.company_name,ss.student_number,s.first_name,s.last_name,
				ip.description, iv.interview_medium_id
		 FROM		job_info as j,
				employer_company as ec,
				students_shortlisted as ss,
				student as s,
				interview_place as ip,
				interview as iv
		 WHERE		iv.interview_place_id=ip.interview_place_id
			AND	ss.student_number=s.student_number
			AND	ec.employer_id=j.employer_id
			AND	j.job_id=\"$job_id\"
			AND	ss.job_id=j.job_id
			AND	iv.job_id=j.job_id
		$order_directive";
	$response = $GLOBALS['dbh']->Execute ($sql);
         while ($row2 = $response->FetchRow())
         {
         $job_code = $row2["job_code"];
         $company_name = $row2["company_name"];
	 //$emailed_employer = $row2["emailed_employer"];
	 //$student_visible = $row2["student_visible"];
         $student_number = $row2["student_number"];
         $first_name = $row2["first_name"];
         $last_name = $row2["last_name"];
         $place_desc = $row2["description"];
         $time = ""; // not known unless signed up for an interview
         $medium = $row2["interview_medium_id"];
         $time_id = -1; // not known unless signed up for an interview
         $room = ""; // not known unles .. bla bla bla

         // are there jobs for today?
/*         $sql = "select int_date from interview_time where job_id=\"$job_id\" and int_date=\"$int_date\"";
         $sqlJOBSFORTODAY = $GLOBALS['dbh']->Execute ($sql);

         // if no jobs, forget it
         if ($sqlJOBSFORTODAY->RecordCount() < 1)
           continue;
*/
         // is the person scheduled for an interview yet on this day?
         $sql = "select student_number from interview_time where job_id=\"$job_id\" and int_date=\"$int_date\"
                 and student_number=\"$student_number\"";
	 $sqlSCHEDULEDFORINTERVIEW = $GLOBALS['dbh']->Execute ($sql);

         $scheduled_for_interview = 0;
         if ($sqlSCHEDULEDFORINTERVIEW->RecordCount() < 1)
         {
           // has the person signed up for an interview on another day?
           $sql = "select student_number from interview_time where job_id=\"$job_id\" and student_number=\"$student_number\"";
           $sqlSIGNUPOTHERDAY = $GLOBALS['dbh']->Execute ($sql);

           // if he has, we don't display it
           if ($sqlSIGNUPOTHERDAY->RecordCount() > 0)
              continue;

           // otherwise everything OK
         }
         else
         {
           // scheduled for interview; get time and time id
           $sql = "select int_time,time_id from interview_time where job_id=\"$job_id\" and student_number=\"$student_number\"";
           $result = $GLOBALS['dbh']->Execute($sql);
           $sqlTIMEID = $result->FetchRow();
           $time = $sqlTIMEID['int_time'];
           $time_id = $sqlTIMEID['time_id'];

           $scheduled_for_interview = 1;
         }

         // get medium of interview
         $sql = "select im.description from interview_medium as im where im.interview_medium_id=\"$medium\"";
         $result = $GLOBALS['dbh']->Execute($sql);
         $sqlMEDIUM = $result->FetchRow();
         $medium_desc = $sqlMEDIUM['description'];

         if ($scheduled_for_interview > 0)
         {
           // get building/room
           $sql = "select it.int_room_id from interview_time as it where it.job_id=\"$job_id\" 
                   and it.student_number=\"$student_number\"";
           $result = $GLOBALS['dbh']->Execute($sql);
           $sqlROOM = $result->FetchRow();

           if ($medium == "1" || $medium == "3") 
           { // In-person interview
             if ($row['interview_place_id'] != 2)
             { // On campus
               if ($sqlROOM['int_room_id'] != 0)
               {
                 $sql = "select bu.building_code,rm.room_number from building as bu,room as rm where
                         bu.building_id=rm.building_id and rm.room_id=\"".$sqlROOM['int_room_id']."\"";

                 $result = $GLOBALS['dbh']->Execute($sql);
                 $sqlLOCN = $result->FetchRow();

                 $room = $sqlLOCN['building_code']." ".$sqlLOCN['room_number'];           
               }
               else
               { // If int_room_id is 0, we don't know where the interview is yet.
                 $room = "TBA";
               }
             }
             else
             { // Off campus
               $sql = "select city from interview_location where job_id=\"$job_id\"";
               $result = $GLOBALS['dbh']->Execute($sql);
               $sqlCITY = $result->FetchRow();

               $room = $sqlCITY['city'];
             }
           }
           else if ($medium == "2")
           { // Phone interview
             if ($time_id == -1)
               $room = "";
             else
             {
               $sql = "select phone from interview_phone where time_id=\"$time_id\"";
               $result = $GLOBALS['dbh']->Execute($sql);
               $sqlPHONE = $result->FetchRow();

               $room = $sqlPHONE['phone'];
             }
           }
           else
           { // No clue
             $room = "?";
           }
         }

	 $lunch_sql="Select c.first_name,c.last_name from contact as c, interview_time as it WHERE c.contact_id=it.student_number AND it.job_id=".$job_id;
	 $lunch_results=$GLOBALS['dbh']->Execute($lunch_sql);
	 $lunch_row=$lunch_results->FetchRow();

	 if ($lunch_results->RecordCount() > 0){
		$lunch=$lunch_row['first_name']." ".$lunch_row['last_name'];
	 }else{
		$lunch="&nbsp;";
	 }

	$href = $PHP_SELF . "&amp;select=view&amp;level1=job_code&amp;level2=job_id_or_job_code&amp;job_id=" . urlencode($job_id) . "&amp;department_id=" . urlencode($department_id);

/*        if (strcmp ($time, ""))
        {
          // date needs beautification, because co-op doesn't work on military time

          $time = formatTime ($time);
        }*/

        $listOfStuff[] = array ("$first_name $last_name", $company_name, $job_code, $time, $room, $medium_desc);
/*
*/	 

      }
    }

      if (!strcmp($sortby, "COMP")) // sort by company
        $listOfStuff = mergesort ($listOfStuff, 1, 1);
      else if (!strcmp($sortby, "JOBC")) // sort by yob code
        $listOfStuff = mergesort ($listOfStuff, 2, 1);
      else if (!strcmp($sortby, "TIME")) // sort by time
        $listOfStuff = mergesort ($listOfStuff, 3, 1);
      else if (!strcmp($sortby, "LOCN")) // sort by location
        $listOfStuff = mergesort ($listOfStuff, 4, 1);
      else if (!strcmp($sortby, "TYPE")) // sort by type
        $listOfStuff = mergesort ($listOfStuff, 5, 1); 
      else // if (!strcmp($sortby, "STUD")) or no sort, just use name 
        $listOfStuff = mergesort ($listOfStuff, 0, 99); 
$rowclass=0;
      foreach ($listOfStuff as $item)
      {
      	if ($item[0] != $lastitem0) {
	($rowclass % 2 == 0) ? $class = "row0d" : $class = "row1d";
	$rowclass++;
	}
							  
         echo("<tr><td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
       	 echo("&nbsp;<a class='blue' href=\"$href\">".$item[0]."</a>&nbsp;</td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">".$item[1]."</a></td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">".$item[2]."</a>&nbsp;</td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">".(strcmp($item[3], "") ? formatTime($item[3]) : "").
              "</a>&nbsp;</td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">".$item[4]."</a>&nbsp;</td>\n");
         echo("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
         echo("&nbsp;<a class='blue' href=\"$href\">".$item[5]."</a></td></tr>\n");
	 $lastitem0=$item[0];
      }

               ?>
                      </table>
                   </td></tr></table>
               <?php

   } 

?>
