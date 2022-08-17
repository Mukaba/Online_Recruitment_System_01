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
 | Filename: edit_header.inc                                                    |
 +------------------------------------------------------------------------------+
 | Description: This file is simply the header file for the edit_job module     |
 | within job_deescriptions. It displays a menu bar across the top of the       |
 | screen allowing the user to choose which part of a job they would like to    |
 | edit.                                                                        |
 +------------------------------------------------------------------------------+

*/

?>

Edit : </td>
	<td width='5'>&nbsp;</td>
	<td valign="middle">&nbsp;<a href="<?php echo($URL . "&amp;level1=jobinfo"); ?>"><b class="blue">Job Info</b></a>&nbsp;</td>
	<td width='5'>&nbsp;</td>
	<td valign="middle">&nbsp;<a href="<?php echo($URL . "&amp;level1=contact"); ?>"><b class="blue">Contact/Company Info</b></a>&nbsp;</td>
	<td width='5'>&nbsp;</td>
	<?php
	
	if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
	{
		echo("<td valign='middle'>&nbsp;<a href='" . $URL . "&amp;level1=host_department'><b class='blue'>Host Department</b></a>&nbsp;</td>");
		echo("<td width='5'>&nbsp;</td>");
	}

	?>
	<td align="left"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td>
	</tr>
</table>
    <?php
        echo("<table width='99%' cellpadding='2'>");
            echo("<tr>");
                echo("<td align='left'>");
                    echo("<h3 align='left'>Job Descriptions - Edit ". $job_code . "</h3>");
                echo("</td>");

                if ($userlevel == OFFICE || $userlevel == TRAFFICDIRECTOR)
                {
                    include_once('job_descriptions/job_quickmenu.inc');
                }
            echo("</tr>");
        echo("</table>");

    ?>
