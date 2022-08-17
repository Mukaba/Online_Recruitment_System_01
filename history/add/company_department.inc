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
| Filename: company_department.inc                                             |
+------------------------------------------------------------------------------+
| Description: This is the screen to go to for placing students that have      |
| found their own jobs. This file allows you to choose which division of a     |
| company the student should be in.                                            |
+------------------------------------------------------------------------------+

*/
$sql = ("
    SELECT ec.employer_id,ed.city,ec.company_name,ed.department_name,ed.department_id 
    FROM employer_company AS ec 
    LEFT JOIN employer_department AS ed 
    ON ed.employer_id=ec.employer_id 
    WHERE ec.company_name LIKE '%".addslashes($addHistory->company_name)."%' AND ec.company_display='1'
    ORDER BY ec.company_name,ed.department_name,ed.city
    "); 
$results = $GLOBALS['dbh']->Execute($sql); 

//display appropriate header: 'own job placement' and 'returning placement' function synonymously; only diff between them is header and page re-routes
if($select == "add_history")
{
    echo("<h3 align='left'>Job Offers - Own Job Placement</h3>");
}
if($select == "returning_placement")
{
    echo("<h3 align='left'>Job Offers - Returning Job Placement</h3>");
}
?>
<h4 align='left'>Select Company and Division for <?php echo($addHistory->first_name ." ". $addHistory->last_name . " (".$addHistory->student_number.")"); ?></h4>
<table cellspacing='0' cellpadding='0' border='1'>
    <tr>
        <td>
            <table cellpadding='2' border='0'>
            <?php
            // re-route to appropriate page: own job or returning job
            if($select == "add_history")
            {
                echo("<form action='" .$PHP_SELF. "&amp;select=add_history' method='POST'>");
            }
            if($select == "returning_placement")
            {
                echo("<form action='" .$PHP_SELF. "&amp;select=returning_placement' method='POST'>");
            }
            ?>
            <tr class='rowgrey'>
                <td align='center'><b class='white'>&nbsp;Company Name&nbsp;</b></td>
                <td align='center'><b class='white'>&nbsp;Division Name&nbsp;</b></td>
                <td align='center'><b class='white'>&nbsp;Place&nbsp;</b></td>
            </tr>
            <?php
            $checked = "checked='checked'";
            $i = 0;
            while($row=$results->FetchRow())
            {
                if ($i % 2)
                {
                    $row_color = "row0d";
                }
                else
                {
                    $row_color = "row1d";
                }

                $company_name=$row['company_name'];
                $department_name=$row['department_name'];
                if ($lastemployer_id==$row['employer_id'])
                {
                    $row['company_name']=NULL;
                }
                $lastemployer_id=$row['employer_id'];
                if ($row['company_name']==NULL)
                {
                    $row['company_name']="&nbsp;";
                }
                if ($row['department_name']==NULL)
                {
                    $row['department_name']="&nbsp;";
                }
                if ($row['city']<>NULL)
                {
                    $row['city']="(".$row['city'].")";
                }
            ?>
                <tr class='<?php echo($row_color); ?>' align='center'>
                    <td>&nbsp;<?php echo $row['company_name']; ?>&nbsp;</td>
                    <td>&nbsp;<?php echo $row['department_name']; ?>&nbsp;<?php echo $row['city']; ?>&nbsp;</td>
                    <td><input type='radio' <?php echo $checked; ?> name='division_id' value="<?php echo $row['department_id']; ?>" /></td>
                </tr>
            <?php
                $checked=NULL;
                $i++;
            }

            if ($i % 2)
            {
                $row_color = "row0d";
            }
            else
            {
                $row_color = "row1d";
            }

            ?>
            <input type='hidden' name='level' value='container_info_form' />
            <input type='hidden' name='addHistory' value='<?php echo(packObject(($addHistory))); ?>' />
            <tr class='<?php echo($row_color); ?>'>
                <td colspan='3'><center><input type='submit' value='Submit' /></center></td>
            </tr>
            </form>
            </table>
        </td>
    </tr>
</table>
