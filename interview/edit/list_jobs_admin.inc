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
 | Filename: list_jobs_admin.inc                                                |
 +------------------------------------------------------------------------------+
 | Description: Show the jobs available to edit, and link to the edit page      |
 +------------------------------------------------------------------------------+

*/

if ( $choice == 'Email shortlist' && $mail[0] )
{
    /*
     Set email_job_id for e-mailing newsgroup.
     */

    $email_job_id = $mail[0];

    /*
     Set email_students array, which contains name, student_number, and 
     e-mail for all students shortlisted for this job.
     */

    $sql = ("
            SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number, a.login_id
            FROM student AS a, students_shortlisted AS b
            WHERE b.job_id='" . addslashes( $email_job_id ) . "'
            AND a.student_number=b.student_number
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        $email_address = getStudentEmail($row["student_number"]);

        $email_students[] = array(
                "name" => $row["name"],
                "student_number" => $row["student_number"],
                "email" => $email_address
                );
    }
    $result->Close();

    /*
     Set email_job array, which contains job_id, job_code, int_dates, and
     the interview medium.
     */

    $sql = ("
            SELECT DISTINCT int_date
            FROM interview_time
            WHERE job_id='" . addslashes($email_job_id) . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        $email_int_dates[] = $row["int_date"];
    }
    $result->Close();

    $sql = ("
            SELECT DISTINCT a.job_code, b.company_name
            FROM job_info AS a, employer_company AS b
            WHERE a.job_id='" . addslashes($email_job_id) . "'
            AND b.employer_id=a.employer_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $result->Close();
    $email_job_code = $row["job_code"];
    $email_company_name = $row["company_name"];

    $sql = ("
            SELECT DISTINCT a.description
            FROM interview_medium AS a, interview AS b
            WHERE b.job_id='" . addslashes($email_job_id) . "'
            AND a.interview_medium_id=b.interview_medium_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $result->Close();
    $email_job_medium = $row["description"];

    /*
     Now we have all the variables, just assign them to the job array.
     */

    $email_job = array(
            "job_id" => $email_job_id,
            "job_code" => $email_job_code,
            "company_name" => $email_company_name,
            "medium" => $email_job_medium,
            "int_dates" => $email_int_dates
            );

    /*
     Now call the send_newsgroup_mail include file.
     */

    $include_after = ("interview/edit/list_jobs_admin.inc");
    include('interview/edit/mail/send_newsgroup_mail.inc');

}
elseif ($choice == 'Email shortlist2' && $mail[0])
{
    /*
     Set email_job_id for e-mailing newsgroup.
     */

    $email_job_id = $mail[0];

    /*
     Set email_students array, which contains name, student_number, and
     e-mail for all students shortlisted for this job.
     */

    $sql = ("
            SELECT DISTINCT CONCAT(a.first_name, ' ', a.last_name) AS name, a.student_number, a.login_id
            FROM student AS a, students_shortlisted AS b
            WHERE b.job_id='" . addslashes( $email_job_id ) . "'
            AND a.student_number=b.student_number
            ");
    $result = $GLOBALS['dbh']->Execute($sql);

    while ($row = $result->FetchRow())
    {
        $email_address = getStudentEmail($row["student_number"]);

        $email_students[] = array(
                "name" => $row["name"],
                "student_number" => $row["student_number"],
                "email" => $email_address
                );
    }
    $result->Close();

    /*
     Set email_job array, which contains job_id, job_code, int_dates, and
     the interview medium.
     */

    $sql = ("
            SELECT DISTINCT int_date
            FROM interview_time
            WHERE job_id='" . addslashes($email_job_id) . "'
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    while ($row = $result->FetchRow())
    {
        $email_int_dates[] = $row["int_date"];
    }
    $result->Close();

    $sql = ("
            SELECT DISTINCT a.job_code, b.company_name
            FROM job_info AS a, employer_company AS b
            WHERE a.job_id='" . addslashes($email_job_id) . "'
            AND b.employer_id=a.employer_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $result->Close();
    $email_job_code = $row["job_code"];
    $email_company_name = $row["company_name"];

    $sql = ("
            SELECT DISTINCT a.description
            FROM interview_medium AS a, interview AS b
            WHERE b.job_id='" . addslashes($email_job_id) . "'
            AND a.interview_medium_id=b.interview_medium_id
            ");
    $result = $GLOBALS['dbh']->Execute($sql);
    $row = $result->FetchRow();
    $result->Close();
    $email_job_medium = $row["description"];

    /*
     Now we have all the variables, just assign them to the job array.
     */

    $email_job = array(
            "job_id" => $email_job_id,
            "job_code" => $email_job_code,
            "company_name" => $email_company_name,
            "medium" => $email_job_medium,
            "int_dates" => $email_int_dates
            );

    /*
     Now call the send_newsgroup_mail include file.
     */

    $include_after = ("interview/edit/list_jobs_admin.inc");
    $email_employer = "pass_through";
    include('interview/edit/mail/send_newsgroup_mail.inc');
}
else
{
    if( $choice == 'Post selected interviews' )
    {
        for( $i=0; $i<count( $post ); $i++ )
        {
            $sql = ("
                    UPDATE interview 
                    SET student_visible=1 
                    WHERE job_id =" . $post[$i]
                   );
            $result = $GLOBALS['dbh']->Execute($sql);
            // Update the student status in the job info table.

            $sql = ("
                    SELECT job_id
                    FROM interview_job_join
                    WHERE interview_job_id='" . $post[$i] . "'
                    ");
            $result = $GLOBALS['dbh']->Execute($sql);

            if ($result->RecordCount())
            {
                while ($row = $result->FetchRow())
                {
                    $sql = ("
                            UPDATE job_info
                            SET student_status='3'
                            WHERE job_id='" . $row["job_id"] . "'
                            ");
                    $result2 = $GLOBALS['dbh']->Execute($sql);
                }
            }
            else
            {
                $sql = ("
                        UPDATE job_info
                        SET student_status='3'
                        WHERE job_id='" . $post[$i] . "'
                        ");
                $result = $GLOBALS['dbh']->Execute($sql);
            }
        }
        $GLOBALS['dbh']->Execute("REPLACE INTO interview_edit (job_id, date_edited, editor, menu_edited)
                VALUES('" . $post[$i] . "', NULL, '" . addslashes($auth->login) . "', 'Button')");
    }
    $curr_id_and_code[] = "";

    $sql = ("
            SELECT DISTINCT a.job_id, c.job_code, a.student_visible, a.emailed_newsgroup, b.int_date, ec.company_name
            FROM interview AS a, interview_time AS b, job_info as c, employer_company as ec 
            WHERE a.job_id=b.job_id 
            AND b.int_date>=CURRENT_DATE 
            AND ec.employer_id=c.employer_id
            AND a.cancelled=0 
            AND c.job_id = a.job_id
            AND c.department_id IN (" . $department_in_str . ") 
            GROUP BY a.job_id, c.job_code, b.int_date
            ORDER BY " . (($order == "company") ? "ec.company_name, c.job_code, b.int_date, a.student_visible, 
            a.emailed_newsgroup" : (($order == "newsgroup") ? "a.emailed_newsgroup, a.student_visible, b.int_date, 
            ec.company_name, c.job_code" : (($order == "posting") ?	"a.student_visible, b.int_date, 
            ec.company_name, c.job_code, a.emailed_newsgroup" : (($order == "job_code") ? "c.job_code, ec.company_name, b.int_date, 
            a.student_visible, a.emailed_newsgroup" : "b.int_date, ec.company_name, c.job_code, a.student_visible, a.emailed_newsgroup" ))))
            );

            $result = $GLOBALS['dbh']->Execute($sql);

            $href = $PHP_SELF . "&amp;select=edit&amp;department_id=" . urlencode($department_id) . "&amp;order=";

            echo( "<form method='post' action='$PHP_SELF&amp;select=edit&amp;order=$order&amp;selected=1'>" );

            if (sizeof($departments_in_group) > 1){
                ?>
                    <table class='row1' cellspacing='0' cellpadding='4' border='0'>
                    <tr>
                    <td>Department:</td>
                    <td><select name="department_id">
                    <?php
                    echo("<option value='0' ". (($department_id=='0') ? "selected='selected'" : "").">All in group</option>");
                foreach($departments_in_group as $dep){
                    echo "\t<option value='" . $dep . "'" . (($dep == $department_id) ? "selected='selected'" : "") . ">" . getDepartmentName($dep) . "</option>\n";
                }
                ?>
                    </select>
                    </td>
                    <td><input type="submit" value="Go" /></td>
                    </tr>
                    </table>
                    <br />
                    <?php
            }
            ?>
                <table cellspacing='0' cellpadding='0' border='1'><tr><td>
                <table border='0' cellpadding='2'>
                <tr>
                <td class='rowgrey' align='center'>&nbsp;&nbsp;<a class="orderable" href="<?php echo($href); ?>date"><b class='white'>Date</b></a>&nbsp;&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;&nbsp;<a class="orderable" href="<?php echo ($href); ?>company"><b class='white'>Company</b></a>&nbsp;&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;&nbsp;<a class="orderable" href="<?php echo($href); ?>job_code"><b class='white'>Job Code(s)</b></a>&nbsp;&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;&nbsp;<a class="orderable" href="<?php echo($href); ?>posting"><b class='white'>Post</b></a>&nbsp;&nbsp;</td>
                <td class='rowgrey' align='center'>&nbsp;<a class="orderable" href="<?php echo($href); ?>newsgroup"><b class='white'>E-mail shortlist</b></a>&nbsp;</td>

                </tr>
                <?php
                $rowclass = 0;

            $unposted = false;
            $unmailed = false;

            while ($row = $result->FetchRow())
            {
                $curr_id_and_code[$row["job_id"]] = $row["job_code"];

                $job_id = $row["job_id"];
                $signed_jobs .= $job_id . ",";
                $href = $PHP_SELF . "&amp;select=edit&amp;job_id=" . $row["job_id"] . "&amp;department_id=" . urlencode($department_id);

                echo("<tr><td class='" . (($rowclass) ? "row0d" : "row1d") . "'>&nbsp;<a class='blue' href='$href'>" . formatStandardDate($row["int_date"]) . "</a>&nbsp;</td>");
                echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "'>&nbsp;<a class='blue' href='$href'>" . $row["company_name"] . "</a>&nbsp;</td>");
                echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "'>&nbsp;<a class='blue' href='$href'>" . $row["job_code"] . "</a>&nbsp;</td>");
                echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>" . (( $row['student_visible'] == 1 )? "<img src='misc/images/smchk_bl.gif' alt='' />":"<input type='checkbox' class='" . (($rowclass) ? "row0d" : "row1d") . "' name='post[]' value='$job_id' />" ) . "</td>");

                echo("<td class='" . (($rowclass) ? "row0d" : "row1d") . "' align='center'>" . (( $row['emailed_newsgroup'] == 1 )? "<img src='misc/images/email.gif' alt='' />":(( $row['student_visible'] == 1 )?"<input type='radio' name='mail[0]' class='" . (($rowclass) ? "row0d" : "row1d") . "' value='$job_id' />": "&nbsp;") ) . "</td></tr>");

                if( $row['student_visible'] != 1 && !$unposted) $unposted = true;
                if( $row['student_visible'] == 1 && $row['emailed_newsgroup'] != 1 && !$unmailed ) $unmailed = true;
                $rowclass = ($rowclass) ? 0 : 1;
            }

            if( $unposted || $unmailed )
            {
                echo ("<tr><td colspan='4' align='right' class='" . (($rowclass) ? "row0d" : "row1d") . "'>" . (( $unposted ) ? "<input type='submit' name='choice' value='Post selected interviews' />" : "&nbsp;") . "</td>");
                echo ( "<td align='right' class='" . (($rowclass) ? "row0d" : "row1d") . "'>". (( $unmailed )? "<input type='submit' value='Email shortlist' name='choice' />" : "&nbsp;") . "</td></tr>");
            }      	
            echo ("</table></td></tr></table></form>");		

} //Done

?>

</td></tr></table>

