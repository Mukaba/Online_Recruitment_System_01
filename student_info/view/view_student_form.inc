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
 | Filename: view_student_form.inc                                              |
 +------------------------------------------------------------------------------+
 | Description: This displays all the student information. Note: This file      |
 | relies on view_student.inc for variables such as record_id, student_num,     |
 | etc.                                                                         |
 +------------------------------------------------------------------------------+

*/

//Show the back and next buttons if it came from advanced search
if ($searchStudents)
{  echo("<center>");
    include("student_info/view/SearchStudents.class");
    echo("<table width='96%' border='0'>");
        echo("<tr align='center'>");
            echo("<td align='left'>");
                echo("<form method='post' action='".$PHP_SELF."&amp;select=view_student'>");
                echo("<input type='hidden' name='continue' value='Chosen Student' />");
                echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
                echo("<input type='hidden' name='student_list' value='".$student_list."' />");
                echo("<input type='hidden' name='record_id' value='".$left_number."' />");
                echo("<input type='image' src='misc/images/previous.gif' />");
                echo("</form>");
            echo("</td>");
            echo("<td align='center'>");
                echo("<form method='post' action='".$PHP_SELF."&amp;select=view_student'>");
                echo("<input type='hidden' name='continue' value='Advanced Search' />");
                echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
                // These two variables are used for paging
                echo("<input type='hidden' name='per_page_max' Value='".$per_page_max."' />");
                echo("<input type='hidden' name='start_row' Value='".$start_row."' />");
                echo("<input type='hidden' name='btnSearch' Value='Search' />");
                echo("<input type='submit' name='back_to_list' value='Back to List' />");
                echo("</form>");
            echo("</td>");
            echo("<td align='right'>");
                echo("<form method='post' action='".$PHP_SELF."&amp;select=view_student'>");
                echo("<input type='hidden' name='continue' value='Chosen Student' />");
                echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
                echo("<input type='hidden' name='student_list' value='".$student_list."' />");
                echo("<input type='hidden' name='record_id' value='".$right_number."' />");
                echo("<input type='image' src='misc/images/next.gif' />");
                echo("</form>");
            echo("</td>");
        echo("</tr>");
    echo("</table>");
echo("</center>");
}

$studentOutput .= ("<table border='1' cellpadding='10' width='100%'>");
    $studentOutput .= ("<tr>");
        $studentOutput .= ("<td>");
            $studentOutput .= ("<table cellpadding='5' cellspacing='0' width='100%'>");

                // Student Name
                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td colspan='3' align='center'>");
                    $studentOutput .= ("<h4>&nbsp;".$editStudent->first_name." ".$editStudent->last_name." - ".$editStudent->student_number . "</h4>");
                    $studentOutput .= ("<hr />");
                    $studentOutput .= ("</td>");
                $studentOutput .= ("</tr>");

                /****** Main Info Section of View Student
                  On Left : Current/Permanent Address, Current/Permanent Phone #, Current/Permanent Fax #, E-mail, High School, Birthdate, Citizenship, Gender.
                  On Right : Photo if it exists
                 *******/
                // Check Photo's existence.
                if ($editStudent->photo == NULL) 
                {
                    $photo_output = ("<b>No Photo Available</b>");
                } 
                else 
                { 
                    $random_num = rand(0,100);
                    $photo_output_html = ("<img src='".$PHP_SELF_MENU."?select=student_photo_viewer&amp;no_html=1&amp;record_id=".$editStudent->record_id."&amp;num=".$random_num."' alt='".$editStudent->first_name." ".$editStudent->last_name."' />");
                    $photo_output_pdf =  ("<img src='".$editStudent->student_number.".jpg' />");
                    $photo_output = STUDENT_PHOTO_PLACEHOLDER;
                }

                // Get Current Address (build the output string..)
                if ($editStudent->street_address_current) 
                {
                    $current_address_output .= ($editStudent->street_address_current . "<br />");
                }

                if ($editStudent->street_address_current2) 
                {
                    $current_address_output .= ($editStudent->street_address_current2 . "<br />");
                }

                if ($editStudent->street_address_current3) 
                {
                    $current_address_output .= ($editStudent->street_address_current3 . "<br />");
                }

                if ($editStudent->city_current) 
                {
                    $current_address_output .= ($editStudent->city_current . ", ");
                }

                $province_current = getProvstateName($editStudent->province_current);
                if ($province_current)
                {	
                    $current_address_output .= ($province_current . "<br />"); 
                }

                else 
                {	
                    $current_address_output .= (($editStudent->city_current) ? "<br />" : ""); 
                }

                $country_current = getCountryName($editStudent->country_current);
                if ($country_current) 
                {
                    $current_address_output .= ($country_current);
                }

                if ($editStudent->postal_code_current) 
                {
                    $current_address_output .= ("<br />" .  $editStudent->postal_code_current);
                }

                // Get Permanent Address (build the output string..)
                if ($editStudent->street_address_permanent) 
                {
                    $permanent_address_output .= ($editStudent->street_address_permanent . "<br />");
                }

                if ($editStudent->street_address_permanent2) 
                {
                    $permanent_address_output .= ($editStudent->street_address_permanent2 . "<br />");
                }

                if ($editStudent->street_address_permanent3) 
                {
                    $permanent_address_output .= ($editStudent->street_address_permanent3. "<br />");
                }

                if ($editStudent->city_permanent) 
                {
                    $permanent_address_output .= ($editStudent->city_permanent . ", ");
                }

                $province_permanent = getProvstateName($editStudent->province_permanent);
                if ($province_permanent)	
                {	
                    $permanent_address_output .= ($province_permanent . "<br />"); 
                }

                else 
                {	
                    $permanent_address_output .=  (($editStudent->city_permanent) ? "<br />" : ""); 
                }

                $country_permanent = getCountryName($editStudent->country_permanent);
                if ($country_permanent) 
                {
                    $permanent_address_output .= ($country_permanent);
                }

                if ($editStudent->postal_code_permanent) 
                {
                    $permanent_address_output .= ("<br />" . $editStudent->postal_code_permanent);
                }

                $col1width = "35%"; 
                $col2width = "35%"; 
                $col3width = "30%";

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Preferred Name:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->preferred_name . "&nbsp;</td>");
                    $studentOutput .= ("<td width='".$col3width."' align='center' valign='middle' rowspan='10'>".$photo_output."</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Middle Name:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->middle_name . "&nbsp;</td>");
                $studentOutput .= ("</tr>");
                
                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Current Address:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $current_address_output . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Current Region:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". getRegionName($editStudent->region_current) . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Current Phone Number:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->phone_current . "&nbsp;</td>");
                $studentOutput .= ("</tr>"); 

                // Added by Adam
                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='" . $col1width . "' align='right' valign='top'><b>Current Fax Number:</b></td>");
                    $studentOutput .= ("<td width='" . $col2width . "' align='left'>" . $editStudent->fax_current . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                // The PDF convertor doesn't like form tags, so we make two seperate output. We break up all the 
                // studentOutput into 3 pieces. $preEmailOutput is everything before the email lines that contain 
                // form tags. $emailOutput is the actual email lines. $pdfEmailOutput is the email lines minus the 
                // form tags. And final, $studentOutput is everything after the email lines. 

                $preEmailOutput = $studentOutput;
                unset($studentOutput);

                $emailOutput .= ("<tr>");
                    $emailOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Primary E-mail:</b></td>");
                    $emailOutput .= ("<td width='".$col2width."' align='left'>");
                    $emailOutput .= ("<form name='emailform1' method='post' action='".$PHP_SELF."'>");
                    $emailOutput .= ("<input type='hidden' name='after' value='student_info/view/view_student.inc' />");
                    $emailOutput .= ("<input type='hidden' name='to' value='".$editStudent->email."' />");
                    $emailOutput .= ("<input type='hidden' name='select' value='email_students' />");
                    $emailOutput .= ("<input type='hidden' name='record_id' value='".$record_id."' />");
                    $emailOutput .= ("<a href='javascript:document.emailform1.submit()'>". $editStudent->email . "</a>");
                        $emailOutput .= ("&nbsp;");
                    $emailOutput .= ("</form>");
                 $emailOutput .= ("</td>");
                $emailOutput .= ("</tr>");

                $emailOutput .= ("<tr>");
                    $emailOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Secondary E-mail:</b></td>");
                    $emailOutput .= ("<td width='".$col2width."' align='left'>");
                    $emailOutput .= ("<form name='emailform2' method='post' action='".$PHP_SELF."'>");
                    $emailOutput .= ("<input type='hidden' name='to' value='".$editStudent->email2."' />");
                    $emailOutput .= ("<input type='hidden' name='select' value='email_students' />");
                    $emailOutput .= ("<input type='hidden' name='record_id' value='".$record_id."' />");
                    $emailOutput .= ("<a href='javascript:document.emailform2.submit()'>". $editStudent->email2 . "</a>");
                        $emailOutput .= ("&nbsp;");
                        $emailOutput .= ("</form>");
                 $emailOutput .= ("</td>");
                $emailOutput .= ("</tr>");

                $pdfEmailOutput .= ("<tr>");
                    $pdfEmailOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Primary E-mail:</b></td>");
                    $pdfEmailOutput .= ("<td width='".$col2width."' align='left'>");
                        $pdfEmailOutput .= ($editStudent->email);
                        $pdfEmailOutput .= ("&nbsp;");
                    $pdfEmailOutput .= ("</td>");
                $pdfEmailOutput .= ("</tr>");

                $pdfEmailOutput .= ("<tr>");
                    $pdfEmailOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Secondary E-mail:</b></td>");
                    $pdfEmailOutput .= ("<td width='".$col2width."' align='left'>");
                        $pdfEmailOutput .= ($editStudent->email2);
                        $pdfEmailOutput .= ("&nbsp;");
                    $pdfEmailOutput .= ("</td>");
                $pdfEmailOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Permanent Address:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $permanent_address_output . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Permanent Region:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". getRegionName($editStudent->region_permanent) . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Permanent Phone Number:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->phone_permanent . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                // Added by Adam
                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='" . $col1width . "' align='right' valign='top'><b>Permanent Fax Number:</b></td>");
                    $studentOutput .= ("<td width='" . $col2width . "' align='left'>" . $editStudent->fax_permanent . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>High School:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->high_school . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Birthdate:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->birth . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Citizen:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->citizen . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right'><b>Gender:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->gender . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr><td colspan='3'><br /><hr /><br /></td></tr>");

                // Start Academic Information
                // On Left : Discipline, Academic Year, Advisor, Expected grad year, admit date, withdraw date, grad date, 
                //           grad gpa
                // On Right : Semesters table displaying the students semesters from start to finish.

                if ($editStudent->start_year == NULL) 
                {
                    $semesters_output = ("No semesters table will show up until a start year is entered for the student");		
                } 
                else
                {
                    $semesters_output .= ("<table border='1' cellspacing='0' cellpadding='1'>");
                    $semesters_output .= ("<tr>");
                    $semesters_output .= ("<td>");
                    $semesters_output .= ("<table cellspacing='1' border='0' cellpadding='3'>");
                    $semesters_output .= ("<tr>");
                    $semesters_output .= ("<td class='row1d'>&nbsp;</td>");

                    // Display the term codes
                    $sql = ("
                        SELECT DISTINCT term_code
                        FROM term
                        ORDER BY order_by
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);
                    while ($row = $result->FetchRow())
                    {
                        $semesters_output .= ("<td class='row1d'>" . $row["term_code"] . "</td>");
                    }

                    $semesters_output .= ("</tr>");
                    $count = 0;

                     
                    // Get all the semesters that the student already is using along with any that the department is 
                    // allowed to set. Then, use these as options for our select menu.

                    $sql = ("
                        SELECT DISTINCT s.semesters_id 
                        FROM semesters AS s
                        INNER JOIN semesters_table AS st ON st.semesters_id=s.semesters_id
                        WHERE st.record_id='" . $editStudent->record_id . "'
                        ORDER BY st.term_id, st.year
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);

                    while ($row = $result->FetchRow()) 
                    {
                        $student_semester_string .= "'" . $row['semesters_id'] . "',";
                    }

                    if ($student_semester_string)
                    {
                        $student_semester_string = substr($student_semester_string, 0, -1);
                    }
                    else
                    {
                        $student_semester_string = "''";
                    }

                    $sql = ("
                        SELECT DISTINCT a.display, a.semesters_id 
                        FROM semesters AS a, department_semesters AS b 
                        WHERE (b.department_id='" . $editStudent->department_id . "' AND b.semesters_id=a.semesters_id) 
                        OR 
                        (a.semesters_id IN (" . $student_semester_string . ")) 
                        ORDER BY display
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);

                    while ($row = $result->FetchRow())
                    {
                        // Build associative array. You supply a semesters_id, it will retun the semester name.  
                        $semesters_row[$row['semesters_id']] = $row['display'];
                    }

                    $editStudent_term_id = getCurrentTermID();
                    $editStudent_year = date("Y");

                    // Determine where to start and end the term iteration.
                    $sql = ("
                        SELECT DISTINCT term_id
                        FROM term
                        ORDER BY order_by
                        ");
                    $result = $GLOBALS['dbh']->Execute($sql);

                    $term_limit = $result->RecordCount() + 1;
                    $row = $result->FetchRow();
                    $term_start = $row["term_id"];
                    $nextYear = $editStudent->start_year;
                    $nextTerm = $term_start;

                    for ($i = $editStudent->start_year; $i < ($editStudent->start_year + 6); $i++)
                    {
                        $semesters_output .= ("<tr>\n");
                            // Format the start year so that a 1998 start year would look like 98-99
                            $semesters_output .= ("<td class='row1d'>");
                            $semesters_output .= (substr($i, -2) . "-" . substr($i + 1 , - 2));
                            $semesters_output .= ("</td>");

                            //for ($j = $term_start; $j < $term_limit; $j++)
                            for ($j = 1; $j < $term_limit; $j++)
                            {
                                if ($nextYear == $editStudent_year && $nextTerm == $editStudent_term_id)
                                {
                                    $semesters_output .= ("<td class='row1'>");
                                }

                                else
                                {
                                    $semesters_output .= ("<td class='row0d'>");
                                }

                                if ($semesters_row[$editStudent->semesters[$nextYear][$nextTerm]])
                                {
                                    $semesters_output .= $semesters_row[$editStudent->semesters[$nextYear][$nextTerm]];
                                }

                                else
                                {
                                    $semesters_output .= "&nbsp;";
                                }

                                $semesters_output .= ("</td>");

                                $nextTermYear = getNextTermYear2($nextTerm, $nextYear);
                                $nextTerm = $nextTermYear["term"];
                                $nextYear = $nextTermYear["year"];
                            }
                            $result->Move(0);

                        $semesters_output .= ("</tr>\n");
                    }

                    $semesters_output .= ("</table>");
                    $semesters_output .= ("</td>");
                    $semesters_output .= ("</tr>\n");
                    $semesters_output .= ("</table>");

                }

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Discipline:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>".getDisciplineName($editStudent->discipline_id)."&nbsp;</td>");
                    $studentOutput .= ("<td width='".$col3width."' align='center' valign='middle' rowspan='8'>".$semesters_output."</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Academic Year:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>".$editStudent->academic_year."&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Faculty Advisor:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->advisor . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $ca_first_name = "";
                $ca_last_name = "";
                if($editStudent->coop_advisor != NULL)
                {
                    $coop_advisor_sql=("Select c.first_name, c.last_name
                        FROM contact c
                        WHERE c.contact_id =".$editStudent->coop_advisor);
                    $coop_advisor_results = $GLOBALS['dbh']->Execute($coop_advisor_sql);
                
                    while ($coop_advisor_row = $coop_advisor_results->FetchRow())
                    {
                        $ca_first_name = $coop_advisor_row["first_name"];
                        $ca_last_name = $coop_advisor_row["last_name"] ;
                    }
                }
                   
                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Co-op Advisor:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $ca_first_name." ".$ca_last_name . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Expected Graduating Year:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->grad_year . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Admittance Date:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->admit . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Withdrawal Date:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->withdraw . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Convocation Date:</b></td>");
                    if($editStudent->convocation_month && $editStudent->convocation_year) 
                    { 
                        $studentOutput .= ("<td width='".$col2width."' align='left'>". date( "F", mktime( 0,0,0,$editStudent->convocation_month+1,0,0 ) ). " " .$editStudent->convocation_year. "&nbsp;</td>");
                    }
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Grad GPA:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->grad_gpa . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr><td colspan='3'><br /><hr /><br /></td></tr>");

                // Flags : Student flags

                // Table Version of Display
                // Build Flags Table.

                $flag_sql = ("
                    SELECT DISTINCT dsf.student_flags_id, sf.description
                    FROM department_student_flags dsf
                    INNER JOIN student_flags sf
                    ON dsf.student_flags_id = sf.student_flags_id
                    WHERE dsf.department_id = '".$editStudent->department_id."'
                    ");
                $flag_results = $GLOBALS['dbh']->Execute($flag_sql);

                $error_in_flags = 0;
                $arrFlags = array();

                // We have no flags for editStudent department. That's not good. 
                if ($flag_results->RecordCount() <= 0)
                {
                    $error_in_flags = 1;
                }

                if (!$error_in_flags)
                {
                    while($flag_row = $flag_results->FetchRow())
                    {
                        $arrFlags[$flag_row['student_flags_id']] = $flag_row['description'];
                    }
                }

                if (sizeof($editStudent->general_flags) > 0 && is_array($editStudent->general_flags) && !$error_in_flags)
                {
                    foreach($editStudent->general_flags as $general_flags)
                    {
                        $flags_output .= $arrFlags[$general_flags]."<br />";
                    }
                }
                
                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td valign='top' align='right'><b>Student Flags:</b></td>");
                    $studentOutput .= ("<td>".$flags_output."</td>");
                    $studentOutput .= ("<td>&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $flags_output = "";

                $term_sql = ("
                    SELECT term_id, term_name
                    FROM term
                    ");
                $term_results = $GLOBALS['dbh']->Execute($term_sql);
                if ($term_results->RecordCount() > 0)
                {
                    $term_names = array();
                    while($term_row = $term_results->FetchRow())
                    {
                        $term_names[$term_row['term_id']] = $term_row['term_name'];
                    }

                    if (sizeof($editStudent->eligible_flags) > 0 && is_array($editStudent->eligible_flags))
                    {
                        $year_keys = array_keys($editStudent->eligible_flags);

                        foreach ($year_keys as $yk)
                        {
                            $term_keys = array_keys($editStudent->eligible_flags[$yk]);
                            foreach ($term_keys as $tk)
                            {
                                if ($editStudent->eligible_flags[$yk][$tk])
                                {
                                    $flags_output .= "Eligible (".$yk." ".$term_names[$tk].")<br />";
                                }
                            }
                        }
                    }
                }

                $placed_sql = ("
                    SELECT ep.term_id, ep.year
                    FROM eligible_placed AS ep, term AS t
                    WHERE ep.record_id = '".$record_id."' AND ep.student_flags_id = '".PLACED_FLAG."'
                    AND ep.term_id=t.term_id
                    ORDER BY ep.year DESC, t.year_order DESC
                    ");
                $placed_results = $GLOBALS['dbh']->Execute($placed_sql);
                if ($placed_results->RecordCount() > 0)
                {
                    while ($placed_row = $placed_results->FetchRow())
                    {
                        $flags_output .= "Placed (".$placed_row['year']." ".$term_names[$placed_row['term_id']].")<br />";
                    }
                }

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td valign='top' align='right'><b>Term Flags:</b></td>");
                    $studentOutput .= ("<td>".$flags_output."</td>");
                    $studentOutput .= ("<td>&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $flags_output = "";

                if (sizeof($editStudent->profile_flags) > 0 && is_array($editStudent->profile_flags) && !$error_in_flags)
                {
                    foreach($editStudent->profile_flags as $profile_flags)
                    {
                        $flags_output .= $arrFlags[$profile_flags]."<br />";
                    }
                }

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td valign='top' align='right'><b>Profile Flags:</b></td>");
                    $studentOutput .= ("<td>".$flags_output."</td>");
                    $studentOutput .= ("<td>&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $flags_output = "";
                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td colspan='3'><br /><hr /><br /></td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'>&nbsp;</td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'><b>Emergency Contact Information</b></td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Name:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->emergency_name . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Relationship:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->emergency_relationship . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Home Phone Number:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->emergency_home_phone . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Work Phone Number:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->emergency_work_phone . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Home Fax Number:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->emergency_home_fax . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Work Fax Number:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->emergency_work_fax . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>E-mail:</b></td>");
                    $studentOutput .= ("<td width='".$col2width."' align='left'>". $editStudent->emergency_email . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr><td colspan='3'><br /><hr /><br /></td></tr>");

                // Last Section : Comments, Notes, Resumes, Coverletters, Work Term History

                // Build Notes
                if ($editStudent->note_ids && is_array($editStudent->note_ids) && sizeof($editStudent->note_ids) > 0)
                {
                    foreach ($editStudent->note_ids as $student_notes_id)
                    {
                        $sql = ("
                            SELECT notes, date_entered, contact_id
                            FROM student_notes
                            WHERE student_notes_id = '".$student_notes_id."'
                            ORDER BY date_entered DESC
                            ");
                        $results = $GLOBALS['dbh']->Execute($sql);
                        $notes_row = $results->FetchRow();
                        
                        $notes_output .= ("<p>");
                        $contact_row = getContactName($notes_row['contact_id']);
                        $contact_name = $contact_row['first_name']." ".$contact_row['last_name'];
                        if ($contact_row['first_name'] && $contact_row['last_name'])
                        {
                            $notes_output .= ("<span class='darktext'>".$contact_name."</span><br />");
                        }
                        $notes_output .= ("<span class='darktext'>".$notes_row['date_entered']."</span><br />");
                        
                        // Replace new lines with line breaks. 
                        $temp=explode("\n",$notes_row['notes']);
                        $notes_output .= (implode("<br />",$temp));

                        $notes_output .= ("</p>");
                    }
                }

                // Build Resume

               $resume_sql = ("
                    SELECT name, resume_id, type_id 
                    FROM resume 
                    WHERE student_number='".$editStudent->student_number."'
                    ");
              
                $get_resumes = $GLOBALS['dbh']->Execute($resume_sql);


                while ($row_resumes = $get_resumes->FetchRow()) 
                {
                    if ($row_resumes['type_id'] == GENERIC_RESUME)
                    {
                        $generic_exists = "true";
                        $generic_resume=$row_resumes["resume_id"];
                    }
                 }
 
                
                if ($generic_exists=="true")
                {
                    $resumes_output .= ("<form name='rform' method='post' action='".$PHP_SELF."'>");
                    $resumes_output .= ("<input type='hidden' name='select' value='resume_transcript_output' />");
                    $resumes_output .= ("<input type='hidden' name='student_num' value='".$editStudent->student_number."' />");
                    $resumes_output .= ("<input type='hidden' name='PDF' value='1' />");
                    $resumes_output .= ("<input type='hidden' name='resume_id' value='".$generic_resume."' />");
                    $resumes_output .= ("<a href='javascript:document.rform.submit()'>GENERIC w/ Transcript</a>");
                    $resumes_output .= ("</form>");
                    $resumes_output .= ("<br />");
                }

                // Build Work Term History

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Notes:</b></td>");
                    $studentOutput .= ("<td colspan='2' align='left'>". $notes_output . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr>");
                    $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Printable Resumes:</b></td>");
                    $studentOutput .= ("<td colspan='2' align='left'>". $resumes_output . "&nbsp;</td>");
                $studentOutput .= ("</tr>");

                $studentOutput .= ("<tr><td colspan='3'><br /><hr /><br /></td></tr>");

                $changes_sql = ("
                    SELECT changes_recorded_1, changes_recorded_2, changes_recorded_3, change_by_1, change_by_2
                    , change_by_3, change_date_1, change_date_2, change_date_3 
                    FROM student_department 
                    WHERE record_id='".$record_id."'
                    ");
                $changes_results = $GLOBALS['dbh']->Execute($changes_sql);
                $changes_row = $changes_results->FetchRow(); 

                if ($changes_row['changes_recorded_1'])
                {
                    $contact_name= getContactName($changes_row['change_by_1']);	
                    
                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Fields Changed:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>". $changes_row['changes_recorded_1'] . "&nbsp;</td>");
                    $studentOutput .= ("</tr>");

                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Changed By:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>".$contact_name['first_name']." ".$contact_name['last_name']."&nbsp;</td>");
                    $studentOutput .= ("</tr>");

                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Change Date:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>". $changes_row['change_date_1'] . "&nbsp;</td>");
                    $studentOutput .= ("</tr>");
                }

                if ($changes_row['changes_recorded_2'])
                {
                    
                    $studentOutput .= "<tr>";
                        $studentOutput .= "<td colspan='3'><br /><hr /><br /></td>";
                    $studentOutput .= "</tr>";

                    $contact_name= getContactName($changes_row['change_by_2']);	

                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Fields Changed:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>".$changes_row['changes_recorded_2'] . "&nbsp;</td>");
                    $studentOutput .= ("</tr>");

                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Changed By:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>".$contact_name['first_name']." ".$contact_name['last_name']."&nbsp;</td>");
                    $studentOutput .= ("</tr>");

                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Change Date:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>".$changes_row['change_date_2'] . "&nbsp;</td>");
                    $studentOutput .= ("</tr>");
                }
                    
                if ($changes_row['changes_recorded_3'])
                {
                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td colspan='3'><br /><hr /><br /></td>");
                    $studentOutput .= ("</tr>");

                    $contact_name= getContactName($changes_row['change_by_3']);	

                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Fields Changed:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>".$changes_row['changes_recorded_3'] . "&nbsp;</td>");
                    $studentOutput .= ("</tr>");

                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Changed By:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>".$contact_name['first_name']." ".$contact_name['last_name']."&nbsp;</td>");
                    $studentOutput .= ("</tr>");

                    $studentOutput .= ("<tr>");
                        $studentOutput .= ("<td width='".$col1width."' align='right' valign='top'><b>Change Date:</b></td>");
                        $studentOutput .= ("<td width='".$col2width."' align='left'>".$changes_row['change_date_3']."&nbsp;</td>");
                    $studentOutput .= ("</tr>");
                }

                // End of Student Info Table
                $studentOutput .= ("</table>");
            $studentOutput .= ("</td>"); 
        $studentOutput .= ("</tr>");
    $studentOutput .= ("</table>");

$pdfOutput = $preEmailOutput.$pdfEmailOutput.$studentOutput;
$htmlOutput = $preEmailOutput.$emailOutput.$studentOutput;

$pdfOutput = str_replace(STUDENT_PHOTO_PLACEHOLDER, $photo_output_pdf, $pdfOutput);

$htmlOutput = str_replace(STUDENT_PHOTO_PLACEHOLDER, $photo_output_html, $htmlOutput);

echo($htmlOutput);
echo("<table width='96%' border='0'>");
    echo("<tr>");

        if ($searchStudents)
        {
            echo("<td align='left'>");
                echo("<form method='post' action='".$PHP_SELF."&amp;select=view_student'>");
                echo("<input type='hidden' name='continue' value='Chosen Student' />");
                echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
                echo("<input type='hidden' name='student_list' value='".$student_list."' />");
                echo("<input type='hidden' name='record_id' value='".$left_number."' />");
                echo("<input type='image' src='misc/images/previous.gif' />");
                echo("</form>");
            echo("</td>");
        }

        if ($auth->userlevel == OFFICE || $auth->userlevel == FACULTY)
        {
            echo("<td align='right'>");
            echo("<form method='post' action='" . $PHP_SELF ."&amp;select=edit_student'>");
            echo("<input type='hidden' name='record_id' value='".$record_id."' />");
            echo("<input type='submit' name='continue' Value='Edit Student' />");
            echo("</form>");
            echo("</td>");
        }

        if ($searchStudents)
        {
            echo("<td align='center'>");

                echo("<form method='post' action='".$PHP_SELF."&amp;select=view_student'>");
                echo("<input type='hidden' name='continue' value='Advanced Search' />");
                echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
                // These two variables are used for paging
                echo("<input type='hidden' name='per_page_max' value='".$per_page_max."' />");
                echo("<input type='hidden' name='start_row' value='".$start_row."' />");
                echo("<input type='hidden' name='btnSearch' value='Search' />");
                echo("<input type='submit' name='back_to_list' value='Back to List' />");
                echo("</form>");
            echo("</td>");
        }

        if ($auth->userlevel == OFFICE)
        {
            echo("<td align='left'>");
        } 
        else
        //$auth->userlevel == FACULTY 
        {
            echo("<td align='center'>");
        }
            echo("<form method='post' action='" . $PHP_SELF . "'>");
                echo("<input type='hidden' name='select' value='studentOutput' />");
                echo("<input type='hidden' name='HTMLStudent' value='". urlencode($pdfOutput) . "' />");
                echo("<input type='hidden' name='PDF' value='1' />");
                echo("<input type='submit' name='viewpdf' value='View in PDF' />");
            echo("</form>");
        echo("</td>");

        if ($searchStudents)
        {
            echo("<td align='right'>");
                echo("<form method='post' action='".$PHP_SELF."&amp;select=view_student'>");
                echo("<input type='hidden' name='continue' value='Chosen Student' />");
                echo("<input type='hidden' name='searchStudents' value='".$searchStudents."' />");
                echo("<input type='hidden' name='student_list' value='".$student_list."' />");
                echo("<input type='hidden' name='record_id' value='".$right_number."' />");
                echo("<input type='image' src='misc/images/next.gif' />");
                echo("</form>");
            echo("</td>");
        }

    echo("</tr>");
echo("</table>");
?>
