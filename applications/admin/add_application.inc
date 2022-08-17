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
 | Filename: add_application.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: This is the main brains behind adding new applicants (paper     |
 | OR online)                                                                   |
 +------------------------------------------------------------------------------+

*/

?>

<script type="text/javascript" language="javascript">
<!--javascript

function popUpStudent(slotName, formName) {

	var str = "document." + formName + ".elements";

	var j = eval(str);
	
	for (var i = 0; i < j.length; i++) {
		var e = j[i];
		
		if (e.name == slotName) {
			var slot = i;
			break;
		}
	}

	window.open("mamook.php?select=student_chooser&no_headers=1&student_list_slot="+slot+"&parentFormName="+formName, "StudentChooser", "toolbar=no,menubar=no,fullscreen=0,top=0,left=0,resizable=yes, width=200, height=200");
}

//-->
</script>

<?php

echo ("<h3>Job Applications</h3>");

if ($apply_method == JOB_APP_METHOD_ONLINE) {
	echo ("<h4>Add Online Applications:</h4>");
}
elseif ($apply_method == JOB_APP_METHOD_PAPER) {
	echo ("<h4>Add Paper Applications:</h4>");
}
else {
	echo ("<h4>Add Special Applications:</h4>");
}

$nextTermYear = getNextTermYear();

$term = $nextTermYear["term"];
$year = $nextTermYear["year"];


//Form items common between the two forms
echo ("<form method='post' action='" . $PHP_SELF . "' name='myform'>");
echo ("<input type='hidden' name='select' value='applications' />");
echo ("<input type='hidden' name='job_id' value='" . $job_id . "' />");
echo ("<input type='hidden' name='apply_method' value='$apply_method' />");



if (($next_step == 'select from list') || ($next_step == 'select_from_list')) {

	//create an array of the names -- had a heck of a time detecting and eliminating empty lines/spaces
	$student_list = trim($student_list);
	$temp_names = explode (chr(10), $student_list);

	$total = 0;
	for ($i =0; $i< count($temp_names); $i++) {
		$hold_names[$i] = explode (",", $temp_names[$i]);
		
		if (!(($hold_names[$i][0] == "") && ($hold_names[$i][1] == ""))
			|| (($hold_names[$i][0] == NULL) && ($hold_names[$i][1] == NULL))
			|| ((ord($hold_names[$i][0]) == 0) && (ord($hold_names[$i][1]) == 0))
			|| ((ord($hold_names[$i][0]) == "0") && (ord($hold_names[$i][1]) == "0"))) {
			$names[$total][0] = trim($hold_names[$i][0]);
			$names[$total][1] = trim($hold_names[$i][1]);
			$total++;
		}
	}

	// this big loop will create a bunch of arrays used later to set up the tables
	// Although it's illogical to create the arrays then use them right away, unfortunately it
	// has to be done this way cause we need to see the results before they're used (for messages),
	// and since that's not possible with a single query, we use the arrays

	$dup = 0; // used for keeping track of duplicates
	$unknown_students = "";
	for ($i =0; $i< count($names); $i++) {

		// this will bring back ONLY CURRENT students
		$sql =  "SELECT DISTINCT s.student_number, CONCAT(s.first_name, ' ', s.last_name) as name, d.department_name,
			ep.student_flags_id
			FROM student_flags_join as sfj, student_department as sd, department as d, student as s
			LEFT JOIN job_info as ji ON ji.job_id = '$job_id'
			LEFT JOIN eligible_placed as ep ON (sd.record_id = ep.record_id AND ji.term_id = ep.term_id AND ji.year = ep.year) 
			WHERE s.first_name LIKE '" . (($names[$i][1] == "") ? "%" : $names[$i][1]."%") . "' 
			AND s.last_name like '" . (($names[$i][0] == "") ? "" : $names[$i][0]."%") . "'
			AND (sfj.student_flags_id = '".CURRENT_FLAG."' AND sfj.record_id = sd.record_id)
			AND (sd.student_number = s.student_number AND d.department_id = sd.department_id)
			ORDER BY s.last_name, s.first_name, d.department_name, s.student_number";

		$result = $GLOBALS['dbh']->Execute ($sql);
		// nifty little trick to set the variable and find out if more than 1 was found
		if (($num_found = $result->RecordCount()) > 1)
			$dup_message = true;

		// didn't find anything, throw it into an array
		if ($num_found < 1) {
			$unknown[] = $names[$i][0] .", ". $names[$i][1];
			$big_arr[$dup][] = array(
				"name" => $names[$i][0] .", ". $names[$i][1],
				"student_number" => -1
				);
			$has_multi = true;
			$dup_message = true;
		}

		// found something
		else {
	
			while ($row = $result->FetchRow()) {

				// see if they have a resume for an online job
				$resresult = $GLOBALS['dbh']->Execute ("SELECT resume_id FROM resume
							WHERE student_number = '". $row["student_number"] ."'");

				// the actual array that will be used
				$big_arr[$dup][] = array(
					"name" => $row["name"],
					"student_number" => $row["student_number"],
					"department_name" => $row["department_name"],
					"eligible" => $row["student_flags_id"],
					"has_resume" => $resresult->RecordCount()
					);
			} // end while
			
			// we use this to know if there are multiples
			if (count($big_arr[$dup]) > 1)
				$has_multi = true;
				
		} // end else

		$dup++;
		
	} // end big for-loop


	// now that we're done the arrays, let's check if there's anything in the $unknown array
	// if there isn't and we're doin an online app, jump to choosing items, otherwise continue
	if (($has_multi) || ($apply_method == JOB_APP_METHOD_PAPER) || ($apply_method == JOB_APP_METHOD_SPECIAL))
		$next_step = 'still_select_from_list';
	
	// need to put in array for choose_items
	else {
		// since there SHOULD only be 1 array under $big_arr[x], it's safe to reference big_arr[x][0] ('0')
		for ($p=0; $p < count($big_arr); $p++)
			$stud_num[] = $big_arr[$p][0]["student_number"];
		$next_step = 'choose_items';
	}

} // done the big if for arrays


// see if we continue
if (($next_step == 'still select from list') || ($next_step == 'still_select_from_list')) {

	if ($apply_method == JOB_APP_METHOD_ONLINE) {
		echo ("<input type='hidden' name='continue' value='add_applicant' />");
		echo ("<input type='hidden' name='next_step' value='choose_items' />");
	}
	else
		echo("<input type='hidden' name='continue' value='add_applicant_now' />\n<br />");
	
	if ($apply_method == JOB_APP_METHOD_PAPER)
		notify ("Applications' status will automatically be set as activated / sent.<br />
            Students who are not eligible will <b class='black'>NOT</b> be set to eligible.<br />
			<b class='black'>'Add More'</b> will add the current applications, then recall any students not found.");
	
	if ($dup_message)
		instruct("Below is a list of students you have entered, including those that were not found. If there are multiple students found, please choose the ones you need."); 
?>
	<br />
	<table cellspacing='0' cellpadding='0' border='1'>
	<tr><td>

	<table border='0' cellpadding='2'>
	
	<tr>
		<td class='rowgrey' align='center'>&nbsp; &nbsp; </td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Name</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Student Number</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Department</b>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<b class='white'>Eligible</b>&nbsp;</td>
	</tr>

	<?php

	$pos = 0; // used for indexing stud_num
	for ($z=0; $z < sizeof($big_arr); $z++) {

		$rowclass ++ ;
		$needChoice = ((sizeof($big_arr[$z]) > 1) ? true : false );

		for ($i=0; $i < sizeof($big_arr[$z]); $i++) {

			$s_name = $big_arr[$z][$i]["name"];
			$s_number = $big_arr[$z][$i]["student_number"];

			// let's see if a student was found, based on what we set their student number to
			if ($s_number == -1) {
		
				echo ("<tr>");
					echo ("<td align='left' colspan='5'>");
					echo ("<table width='100%' class='error' cellpadding='0' cellspacing='0' border='0'>");
					echo ("<tr><td> &nbsp;Not found: $s_name </td></tr>");
					echo ("</table>");
					echo ("</td>");
				echo ("</tr>");
			
			}


			// a student was found, shoot out their info
			else {
			
				$d_name = $big_arr[$z][$i]["department_name"];
				$eligible = $big_arr[$z][$i]["eligible"];
				$has_resume = $big_arr[$z][$i]["has_resume"];

				// the tables' rows
				echo("<tr>");

					echo ("<td align='center' class='" . (($rowclass % 2) ? "row1d" : "row0d" ) . "'>&nbsp;");
					if (($eligible != ELIGIBLE_FLAG) && ($apply_method != JOB_APP_METHOD_SPECIAL)) {
						echo ("<b class='red'>X</b>");
                    }
					elseif ((!$has_resume) && ($apply_method == JOB_APP_METHOD_ONLINE)) {
						echo ("<b class='red'>N/R</b>");
						$need_nr = true;
					}
					elseif ($needChoice) {
						echo ("<input type='radio' name='stud_num[$pos]' value='$s_number' class='" . (($rowclass % 2) ? "row1d" : "row0d" ) . "' />");
						$need_increase = true;
					}
					else {
						echo ("<input type='hidden' name='stud_num[$pos]' value='$s_number' />");
						$need_increase = true;
					}
					
					echo ("&nbsp;</td>");

					echo ("<td align='left' class='" . (($rowclass % 2) ? "row1d" : "row0d" ) . "'>");
					echo ("&nbsp; $s_name&nbsp; &nbsp; ");
					echo ("</td>");

					echo("<td align='center' class='" . (($rowclass % 2) ? "row1d" : "row0d" ) . "'>");
					echo("&nbsp;$s_number&nbsp;");
					echo ("</td>");

					echo("<td align='center' class='" . (($rowclass % 2) ? "row1d" : "row0d" ) . "'>");
					echo("&nbsp;$d_name&nbsp;");
					echo ("</td>");

					echo("<td align='center' class='" . (($rowclass % 2) ? "row1d" : "row0d" ) . "'>");
					echo((($eligible == ELIGIBLE_FLAG) ? "Yes" : "<b class='red'>No</b>" ));
					echo ("</td>");

				echo("</tr>\n");

			} // if student 
		} //  for-loop $i

		// only increase when needed (radio or hidden used)
		if ($need_increase) {
			$pos ++;
			$need_increase = false;
		}

	} // for-loop $z


	echo ("</td></tr></table></table>");
	if (($need_nr) && ($apply_method == JOB_APP_METHOD_ONLINE))
		echo ("N/R - Student does not have a resume so you can not apply them for a job.");

	echo ("<br /><br />");
	if ($apply_method == JOB_APP_METHOD_ONLINE) {
		if (count($unknown) > 0)
			echo ("<input type='hidden' name='unknown_students' value='". packObject(($unknown)) ."' />");
		echo ("<input type='submit' value=' Continue ' />");
	}

	else {
		echo ("<input type='submit' value=' Confirm ' />");

		if (is_array($unknown) && (count($unknown) > 0)) 
			echo ("<input type='hidden' name='unknown_students' value='". packObject(($unknown)) ."' />");
		
		echo (" &nbsp; &nbsp; <input type='submit' name='add_more' value=' Add More ' />");
	}

	echo ("</form>");
}



// ONLINE APPLICATIONS - choosing items
elseif ($next_step == 'choose_items') {

?>
	<input type='hidden' name='continue' value='add_applicant_now' />

<?php	if ($apply_method == JOB_APP_METHOD_ONLINE)
                notify ("Applications' status will automatically be set as activated / sent.<br />
			<b class='black'>'Add More'</b> will add the current applications, then recall any students not found.");

	instruct ("Please choose the items for each student's application."); 
	echo ("<input type='hidden' name='apply_method' value='$apply_method' />");?>

	<br />
	<table cellspacing='0' cellpadding='0' border='1'>
	<tr><td>

	<table border='0' cellpadding='2'>
		<tr>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Name</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Student Number</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Coverletter</b>&nbsp;</td>
			<td class='rowgrey' align='center'>&nbsp;<b class='white'>Resume</b>&nbsp;</td>
			<td class='rowgrey' align='center'><b class='white'>Include Transcript</b></td>
		</tr>
<?php
	$rowclass = 0;
	$pos = 0;
	for ($n=0; $n < count ($stud_num); $n++) {

		$rowclass ++;
		$number = $stud_num[$n];
		
		// similar to way above, if student number is -1, means they weren't found
		if ($number == -1) {

		  
			echo ("<tr>");
				echo ("<td align='left' colspan='5'>");
				echo ("<table width='100%' class='error' cellpadding='0' cellspacing='0' border='0'>");
				echo ("<tr><td> &nbsp;Not found: $s_name </td></tr>");
				echo ("</table>");
				echo ("</td>");
			echo ("</tr>");
		   
		}
		
		else {
			echo ("<tr>");

			// see if the student already has an application
			$has_app = $GLOBALS['dbh']->Execute ("SELECT application_id FROM applications
						WHERE student_number = '$number' AND job_id = '$job_id'");
			$already = $has_app->RecordCount();

			// get the student's name and display it
			$nresult = $GLOBALS['dbh']->Execute ("SELECT CONCAT(first_name, ' ', last_name) as name from student 
						where student_number = '$number'");
			$nrow = $nresult->FetchRow();

			echo ("<td class='". (($rowclass % 2) ? "row1d" : "row0d" ) ."' align='left'>");
			echo ("&nbsp; ". $nrow["name"] ."&nbsp; &nbsp; ");
			echo ("</td>");
			
			
			// display their student number
			echo ("<td class='". (($rowclass % 2) ? "row1d" : "row0d" ) ."' align='center'>");
			if (!$already)
				echo ("<input type='hidden' name='stud_num[$pos]' value='$number' />");
			echo ("&nbsp;$number&nbsp;");
			echo ("</td>");
		
			if ($already) {
				echo ("<td align='center' colspan='3' class='". (($rowclass % 2) ? "row1d" : "row0d" ) ."'>");
				echo (" --- Application already exists --- ");
				echo ("</td>");
			}

			else {
				// find all their coverletters and display them in a drop down
				$cresult = $GLOBALS['dbh']->Execute ("SELECT type_id, coverletter_id from coverletter 
							where student_number='$number' 
							AND type_id = '".GENERIC_COVERLETTER."'");

				echo ("<td class='". (($rowclass % 2) ? "row1d" : "row0d" ) ."' align='left'>");
				if ($cresult->RecordCount()) {	
			
					echo ("&nbsp; <select name='coverletter_id[$pos]'>");
					while ($crow = $cresult->FetchRow()) {
						echo ("<option value='". $crow["coverletter_id"] ."' ".  (($crow["type_id"] == 3) ? "selected='selected'" : "") .">");
						echo ((($crow["type_id"] == 3) ? " Generic " : " Template ") ."</option>");
					}
					echo ("<option value='0'> None </option>");
					echo ("</select> &nbsp;");
				}

				else
					echo (" -- None -- ");
				
				echo ("</td>");
			
			
				// find all their resumes and display them in a drop down
				$rresult = $GLOBALS['dbh']->Execute ("SELECT name, resume_id, type_id from resume where student_number = '$number'");
			
				echo ("<td class='". (($rowclass % 2) ? "row1d" : "row0d" ) ."' align='left'>");
				if ($rresult->RecordCount()) {
				
					echo ("&nbsp; <select name='resume_id[$pos]'>");
					while ($rrow = $rresult->FetchRow()) {
						echo ("<option value='". $rrow["resume_id"] ."' ". (($rrow["type_id"] == 3) ? "selected='selected'" : "") .">");
						echo (" ". $rrow["name"] ." </option>");
					}
					echo ("</select> &nbsp;");
				}

				else
					echo (" -- None -- ");
				echo ("</td>");


				// display a checkbox for transcript
				echo ("<td class='". (($rowclass % 2) ? "row1d" : "row0d" ) ."' align='center'>");
					echo("<input type='checkbox' name='inc_trans[$pos]' value='1' class='". (($rowclass % 2) ? "row1d" : "row0d" ) ."' checked ='checked' />");
				echo ("</td>");
			
				$pos ++; // increment ONLY if no application
			} //  already has app
			echo ("</tr>\n");

		} // else is student
	} // for loop

	echo ("</table>\n</td></tr></table><br /><br />");


	echo ("<input type='submit' value=' Confirm ' />");

	if (count($unknown_students) > 0)
		echo ("<input type='hidden' name='unknown_students' value='$unknown_students' />");
	
	echo (" &nbsp; &nbsp; <input type='submit' name='add_more' value=' Add More ' />");
	echo ("</form>");
	

}



// ENTERING NAMES
elseif (!$next_step || ($next_step == 'enter_names') || ($next_step == 'enter names') || ($has_unknown_students)) {

	echo ("<input type='hidden' name='next_step' value='select_from_list' />");
	echo ("<input type='hidden' name='continue' value='add_applicant' />");

	// if they've chosen to add more, it will bring up these messages, otherwise it takes them to the applicant
	// list where it will alternatively display these messages
	if (($add_more) && (count($unknown_students) > 0)) {
		// NOTE: these also appear in add_application.inc
		if ($successful)
			notify ("Applications for the following students have been added successfully: <br />$successful");
		error ( (($unsuccessful != "") ? "Errors were encountered when adding applications for the following students:<br />$unsuccessful<br />" : "") 
			."The following students could not be found:$unknown_notice");
		echo ("<br />");
	}

	echo("<table border='0' cellpadding='5' cellspacing='0' width='70%' class='row1'>");

		echo("<tr>");
			echo("<td align='center' colspan='2'>");
			echo ("Please enter the names of the students who have applied, <br />separated by a new line in the ");
			echo ("format: <br /><b class='black'>Last name, First name</b><br />");
			echo ("You may use full names or partial names.<br /><br />");
			echo ("Or you may insert students by selecting them from a list. <br /><br />");
			echo ("<input type='button' name='insert_from_list' value='Insert from list' onclick='popUpStudent(\"student_list\", \"myform\");' /><br /><br />");
			echo ("<textarea name='student_list' rows='16' cols='40'>$unknown_text</textarea>");
			echo ("</td>");
		echo ("</tr>");
		echo ("<tr>");
			echo ("<td colspan='2' align='center'><hr /></td>");
		echo ("</tr>");
		echo ("<tr>");
			echo ("<td align='right' width='50%'>");
			echo ("<input type='submit' value='Continue' /> ");
			echo ("</td>");

			echo ("<td align='left' width='50%'>");
			echo ("<input type='button' value='Cancel' onclick='javascript:document.cancelform.submit();' />");
			echo ("</td>");

		echo("</tr>");

	echo("</table>");
    echo ("</form>");

    echo ("<form method='post' action='" . $PHP_SELF . "' name='cancelform'>");
    echo ("<input type='hidden' name='select' value='apply_job' />");
    echo ("<input type='hidden' name='continue' value='view_applicants' />");
    echo ("<input type='hidden' name='job_id' value='" . $job_id . "' />");
    echo ("</form>");

}
?>
