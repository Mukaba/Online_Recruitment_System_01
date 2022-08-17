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
 | Filename: add.inc                                                            |
 +------------------------------------------------------------------------------+
 | Description: Displays a page where the user can enter the job code for the   |
 | coverletter they wish to add, or select to add a generic or template         |
 | coverletter if they do not have one.  If there are no resumes in the system  |
 | for the student the student can not add a job specific coverletter until     |
 | they have  at least one resume in the system.                                |
 +------------------------------------------------------------------------------+

*/


$template = 1;
$generic = 1;

$sql = "SELECT count(letter) as count, type_id
	FROM coverletter
	WHERE student_number = '$student_number'
	GROUP BY type_id";

$result = $GLOBALS['dbh']->Execute( $sql );

while ($row = $result->FetchRow()){
	if ($row["type_id"] == TEMPLATE && $row["count"] > 0){
		$template = "0";
	}elseif($row["type_id"] == GENERIC && $row["count"] > 0){
		$generic = "0";
	}
}
?>
<form method="post" action="<?php echo $PHP_SELF?>" name="form">
<input type="hidden" value="coverletter" name="select" />

<!-- default the page to Add Job incase enter is pressed instead of return-->
<input type="hidden" value="Add Job" name="page" />

<table cellspacing="0" cellpadding="0" border="0">
  <tr>
    <td><h3>Add Cover Letter</h3></td>
  </tr>
  <tr>
    <td>
	<?php if (($job_code == "")) { ?>
      <table  border="1" cellspacing="0" cellpadding="5">
	  <?php
	    if ($generic){
	  ?>
	  	<tr class='row0d'><td align="center" width="50%"><input type="submit" value="Add Generic" name="page" /></td>
			<td><b>Add a Generic Cover Letter - </b> This cover letter will be sent out whenever a specific cover letter is not provided.</td></tr>
	  <?php
	    }
	    if ($template){
	  ?>
	  	<tr class='row0d'><td align="center"><input type="submit" value="Add Template" name="page" /></td>
			<td><b>Add a Template Cover Letter -</b> This cover letter will never be sent out, it is simply used for you to base your specific cover letters off of.</td></tr>
	  <?php
	    }
	  ?>
      </table>
	<?php } ?>
    </td>
  </tr>

  <tr class='row0d'>
	<td>
		<br />
		<p>If you would like to add a job specific cover letter, please do so by clicking the 'Apply to Job' button through Job Descriptions.</p>
	</td>
  <tr>

<!--  <tr class='row0d'>
    <?php
    $sql = "SELECT name, resume_id
    	    FROM resume
	    WHERE student_number = '" . addslashes($student_number) . "'
	    ORDER BY type_id DESC, name ASC";
    $result = $GLOBALS['dbh']->Execute($sql);
    
    if ($result->RecordCount() <= 0 ){
	    ?>
	<td>
	  <p>You cannot add a job specific cover letter until you have at least one resume in the system. Click on the 'Add' option under the 'Resume' menu to add a resume to the system.</p>
	</td>
    <?php
    }else{
	    ?>
    
 	    <td>

	     <table width="100%" border="1" cellspacing="0" cellpadding="5">
 	       <tr>
		  <td>
			<b>Add a Specific Cover Letter - </b> 
			You must specify a job code and resume to write a specific cover letter. 
			This cover letter will be sent along with the resume to the specified job code.<br />
			<br /><hr size="1" width="60%"><br />


		    <table align="left" border="0" cellspacing="0" cellpadding="5">
		      <tr>
		        <td align="left">Job Code:</td>
			<td align="left"><input type="text" size="20" value="<?php echo $job_code?>" name="job_code"></td>
			<td>&nbsp; &nbsp; Insert the <b>job code</b> of the job you wish to apply for.</td>
		      </tr>
		      <tr>
		        <td align="left">Resume:</td>
			<td align="left"><select name="resume_id">
			  <?php
			  while ($row = $result->FetchRow()){
				  ?>
				  <option value="<?php echo $row["resume_id"]?>"><?php echo $row["name"]?></option>
				  <?php
			  }
			  ?> 
			  	</select>
			</td>
			<td> &nbsp; &nbsp; Select the <b>Resume</b> you wish to send along with the application.</td>
		      </tr>
		      <tr>
		        <td colspan="2">
				<?php
				if (!$generic){
					?>
					<input type="CHECKBOX" name="use_generic" />&nbsp;Use Generic Cover Letter<br /> 
					<?php
				}
					?>
				<input type="CHECKBOX" name="suppress_transcript" />&nbsp;Suppress Transcript
				</td><td>&nbsp; &nbsp; <?php if (!$generic) { ?>Check this if you wish to send your <b>generic cover letter</b>, <br />&nbsp; &nbsp; if not you will be asked to create a specific one. <?php } ?></td>
		      </tr>
		      <tr>
		        <td><input type="submit" value="Add Job" name="page" /></td>
			<td><input type="submit" value="Search for Job" name="page" /></td>
			<td> &nbsp; </td>
		      
		      </tr>
		    </table>
		  </td>
		</tr>
	      </table>

	    </td>
    	<?php
    }
    ?>
  </tr> -->
</table>
