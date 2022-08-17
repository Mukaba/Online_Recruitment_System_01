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
 | Filename: employer_job_choice.inc                                            |
 +------------------------------------------------------------------------------+
 | Description: This file loads up all of the employer's company's jobs, and    |
 | allows them to select one of them to edit.                                   |
 +------------------------------------------------------------------------------+

*/

$nextTermYear = getNextTermYear();

$show_past = urldecode($show_past);
$show_future = urldecode($show_future);


if (($userlevel == ADMINEMPLOYER) || ($userlevel == EMPLOYER)) {

	$sql = "SELECT ";
	
	// used later on (if needed) for the drop down of different pages
	switch ($order) {
		case 'term_year':
			$sql .= "CONCAT(t.term_name, ', ', ji.year) as term_year,
				ji.closing_date, ji.employer_code, ji.position_title, ji.job_code, ";
			break;
			
		case 'coop_code':
			$sql .= "ji.job_code, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date, 
				ji.employer_code, ji.position_title, ";
			break;
		
		case 'employer_code':
			$sql .= "ji.employer_code, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date,
				ji.position_title, ji.job_code, ";
			break;

		case 'position_title':
			$order = 'position_title';
			$sql .= "ji.position_title, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.closing_date,
				ji.employer_code, ji.job_code, ";
            break;

		default:
		case 'closing_date':
			$sql .= "ji.closing_date, CONCAT(t.term_name, ', ', ji.year) as term_year, ji.employer_code, 
				ji.position_title, ji.job_code, ";
			break;
	}

	$sql .= "ji.job_id, ji.status
		FROM job_info ji
        LEFT JOIN contact_employer ce
          ON ji.employer_department_id = ce.department_id
        INNER JOIN term t
          ON ji.term_id = t.term_id
		WHERE ji.employer_id = '" . $employer_id . "'
            AND ji.year >= '".(date("Y") - 2)."'
		    AND ce.contact_id = '".(($userlevel == EMPLOYER) ? $contact_id : $employer_contact_id_login)."'
		AND ji.position_title != ''
		AND ji.position_title is not null
		AND ji.status NOT IN ('". INT_ONLY ."', '". POSTED ."', '". REJECTED ."')";

	$sql .= " ORDER BY ";
	switch ($order) {
		case 'term_year':
			$sql .= "ji.year DESC, t.year_order, ji.closing_date DESC, ji.employer_code, ji.position_title ";
			break;

		case 'coop_code':
			$sql .= "ji.job_code, ji.year DESC, ji.term_id, ji.closing_date DESC, ji.employer_code, ji.position_title ";
			break;
		
		case 'employer_code':
			$sql .= "ji.employer_code, ji.position_title, ji.year DESC, ji.term_id, ji.closing_date DESC ";
			break;

		case 'position_title':
			$sql .= "ji.position_title, ji.employer_code, ji.year DESC, ji.term_id, ji.closing_date DESC ";
			break;

		default:
		case 'closing_date':
			$sql .= "ji.closing_date DESC, ji.year DESC, ji.term_id, ji.employer_code, ji.position_title ";
			break;
	}
}
else {
	include ('misc/loguser.inc');
}

echo ("<h4>Add a new job from scratch:</h4>");
echo ("<form method='post' action='" . $PHP_SELF . "'>");
echo ("<input type='hidden' name='select' value='add_job' />");
echo ("<input type='hidden' name='employer_start_scratch' value='true' />");
echo ("<input type='submit' value='Add from scratch' />");
echo ("</form>");

echo ("<h4> - OR - </h4>");

echo ("<h4>Select from the following to use as a template:</h4>");

$result = $GLOBALS['dbh']->Execute($sql);

$num_jobs = $result->RecordCount();


if ($num_jobs == 0) {
	// if there are no jobs in the database, indicate this 

	notify("You do not have any jobs in the system.\n");
} 

else {
	// there are jobs in the system

	if ($jobs_per_page == "") { $jobs_per_page = 20; }
	if ($jobs_per_page < 5) { $jobs_per_page = 5; }
	if ($job_page == '') { $job_page = 0; }
	

	// display the thingy if necessary
	if ($num_jobs > 20) {

		$num_pages = ceil($num_jobs / $jobs_per_page);
		
		$job_pos_start = $job_page * $jobs_per_page;
		$job_pos_end = $job_pos_start + $jobs_per_page;

		$sql .= "LIMIT $job_pos_start, $jobs_per_page";

		echo ("<form method='post' action='$PHP_SELF'>");
		echo ("<input type='hidden' name='order' value='$order' />");
		echo ("<table border='0' class='row1' cellspacing='0' cellpadding='4'>");
			echo ("<tr>");
				echo ("<td valign='bottom'>");
				echo ($num_jobs . " job" . (($num_jobs != 1) ? "s" : "") . " on " . $num_pages . " page" . (($num_pages > 1) ? "s" : "") .";");
				echo ("</td>");
				
				echo ("<td valign='bottom'>");
				echo ("Show <input type='text' name='jobs_per_page' size='3' value='$jobs_per_page' /> jobs per page");
				echo ("</td>");
			echo ("</tr>");
			
			echo ("<tr>");
				echo ("<td>");
				echo ("<select name='job_page'>");
				
				for ($j = 0; $j < $num_pages; $j ++) {
				
					$from_pos = $j * $jobs_per_page;
					$to_pos = $from_pos + $jobs_per_page - 1;
					if ($to_pos >= $num_jobs) $to_pos = $num_jobs - 1;

                    $result->Move($from_pos);
                    $result_row = $result->FetchRow();
                    $from = $result_row[0];

                    $result->Move($to_pos);
                    $result_row = $result->FetchRow();
                    $to = $result_row[0];

					// special cases to concatenate the strings
					if ($order == 'term_year') {
						$from = substr($from, 0, 2) . ". " . substr($from, -4, 4);
						$to = substr($to, 0, 2) . ". " . substr($to, -4, 4);
					}
					
					else {
						if (strlen($from) > 12) { $from = substr($from, 0, 12) . "..."; }
						if (strlen($to) > 12) { $to = substr($to, 0, 12) . "..."; }
					}
					
					echo ("<option value='$j'". (($j == $job_page) ? " selected='selected'" : "") .">");
					echo ("$from  to  ". (($to == '') ? "n/a" : $to));
					echo ("</option>");
				}
				echo ("</select>");
				echo ("</td>");

				echo ("<td align='right'>");
				echo ("<input type='submit' value='View' />");
				echo ("</td>");
			echo ("</tr>");
		echo ("</table>");
		echo ("</form>");
	}

	$result = $GLOBALS['dbh']->Execute($sql);

	if ($result->RecordCount() != 0) {

	// arrows
	if ($num_jobs > 20) {
		echo ("<table cellspacing='0' cellpadding='0' border='0'>");
		echo ("<tr>");
			echo ("<td align='left'>");
		
			if ($job_page > 0) {
				echo ("<form method='post' action='$PHP_SELF'>");
				echo ("<input type='hidden' name='order' value='$order' />");
				echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
				echo ("<input type='hidden' name='job_page' value='". ($job_page - 1) ."' />");
				echo ("<input type='submit' value='<' />");
				echo ("</form>");
			} else {
				echo ("&nbsp;");
            }
				
			echo ("</td>");
			echo ("<td align='right'>");

			if ($job_page < ($num_pages - 1)) {
				echo ("<form method='post' action='$PHP_SELF'>");
				echo ("<input type='hidden' name='order' value='$order' />");
				echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
				echo ("<input type='hidden' name='job_page' value='". ($job_page + 1) ."' />");
				echo ("<input type='submit' value='>' />");
				echo ("</form>");
			} else {
				echo ("&nbsp;");
            }
				
			echo ("</td>");
		echo ("</tr>");

		echo ("<tr>");
			echo ("<td colspan='2'>");
	} // arrows
	
		while ($row = $result->FetchRow())
		{
			$position_title = $row["position_title"];
			$closing_date = (($row["closing_date"] == "0000-00-00") ? "Not Yet Set" : $row["closing_date"]);
			$coop_code = (($row["job_code"] == '') ? "Not Yet Set" : $row["job_code"]);
			$employer_code = (($row["employer_code"] == '') ? "n/a" : $row["employer_code"]);
			$job_id = $row["job_id"];
			$term_year = $row["term_year"];

			if (($employer_code != 'n/a') && (!$has_employer_code))
				$has_employer_code = true;

			$jobs_array[] = array (
					"position_title" => $position_title,
					"closing_date" => $closing_date,
					"coop_code" => $coop_code,
					"employer_code" => $employer_code,
					"job_id" => $job_id,
					"term_year" => $term_year,
					);
		}
		
	
		$href = $PHP_SELF ."&amp;jobs_per_page=$jobs_per_page&amp;job_page=$job_page&amp;show_term=$show_term&amp;show_year=$show_year&amp;order=";
		
		echo ("
		<table cellspacing='0' cellpadding='0' border='1'><tr><td>
		<table border='0' cellpadding='2'>
		<tr>");
		echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."position_title'><b class='white'>Position Title</b></a>&nbsp;</td>");
		if ($has_employer_code)
			echo ("<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."employer_code'><b class='white'>Your Job Code</b></a>&nbsp;</td>");
		echo ("
		<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."coop_code'><b class='white'>Co-op Job Code</b></a>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."term_year'><b class='white'>Job Term/Year</b></a>&nbsp;</td>
		<td class='rowgrey' align='center'>&nbsp;<a class='orderable' href='$href"."closing_date'><b class='white'>Closing Date</b></a>&nbsp;</td>
		</tr>
		");
        
		for ($rowclass = 0; $rowclass < count($jobs_array); $rowclass ++) {

			$position_title = $jobs_array[$rowclass]["position_title"];
			$closing_date = $jobs_array[$rowclass]["closing_date"];
			$coop_code = $jobs_array[$rowclass]["coop_code"];
			$employer_code = $jobs_array[$rowclass]["employer_code"];
			$job_id = $jobs_array[$rowclass]["job_id"];
			$term_year = $jobs_array[$rowclass]["term_year"];

			
			$href = $PHP_SELF . "&amp;select=add_job&amp;continue=template_job_chosen&amp;job_id=" . urlencode($job_id);

			echo ("<tr>");
			echo ("<td align='left' class='" . (($rowclass % 2) ? "row0d" : "row1d" ) . "'>");
			echo ("&nbsp;<a class='blue' href=\"$href\">$position_title</a>&nbsp;</td>\n");
			if ($has_employer_code) {
				echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
				echo ("&nbsp;<a class='blue' href=\"$href\">$employer_code</a>&nbsp;</td>\n");
			}
			echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo ("&nbsp;<a class='blue' href=\"$href\">$coop_code</a>&nbsp;</td>\n");
			echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo ("&nbsp;<a class='blue' href=\"$href\">$term_year</a>&nbsp;</td>\n");
			echo ("<td align='center' class='" . (($rowclass % 2) ? "row0d" : "row1d" ). "'>");
			echo ("&nbsp;<a class='blue' href=\"$href\">$closing_date</a>&nbsp;</td>\n");
			
			echo ("</tr>\n");
		}
		echo ("</table>");
		echo ("</td></tr></table>");
	
	// arrows
	if ($num_jobs > 20) {
	
			echo ("</td>");
		echo ("</tr>");
		
		echo ("<tr>");
			echo ("<td align='left'>");
		
			if ($job_page > 0) {
				echo ("<form method='post' action='$PHP_SELF'>");
				echo ("<input type='hidden' name='order' value='$order' />");
				echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
				echo ("<input type='hidden' name='job_page' value='". ($job_page - 1) ."' />");
				echo ("<input type='submit' value='<' />");
				echo ("</form>");
			} else {
				echo ("&nbsp;");
            }
				
			echo ("</td>");
			echo ("<td align='right'>");

			if ($job_page < ($num_pages - 1)) {
				echo ("<form method='post' action='$PHP_SELF'>");
				echo ("<input type='hidden' name='order' value='$order' />");
				echo ("<input type='hidden' name='jobs_per_page' value='$jobs_per_page' />");
				echo ("<input type='hidden' name='job_page' value='". ($job_page + 1) ."' />");
				echo ("<input type='submit' value='>' />");
				echo ("</form>");
			} else {
				echo ("&nbsp;");
            }
				
			echo ("</td>");
		echo ("</tr>");
		echo ("</table>");
	} // arrows
	
	} 

}

?>
