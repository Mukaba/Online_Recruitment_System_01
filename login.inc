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
 | Filename: login.inc                                                          |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: login.inc
?>
   </td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>" alt="" /></td></tr></table>
<br />   

<?php

// If we got an error message (will only happen if this person has been blacklisted), display it.

if ($error_msg)
{
	error($error_msg);
}

?>

<table border="0" cellspacing="0" cellpadding="20" width="100%"><tr><td>
<div align="center">
   <h2>Co-op at <?php echo(ORGANIZATION_FULL) ?></h2>
   <br />
   <h5>Welcome to <?php echo(BRAND_NAME); ?>.  Please log in using your assigned ID and password:</h5> 
<?php
if ($bad_login == "wrong_pass"){
	echo "<table class='notify' border='0' cellspacing='0' cellpadding='0' width='50%'><tr><td nowrap='nowrap'>";
        echo "<center><h3>Invalid login. Please try again.</h3></center></td></tr></table>";
}	
?>
   <form action="<?php echo($PHP_SELF); ?>" method="post" name="loginform">
      <input type="hidden" name="authcheck" value="1" />
      <table border="0" cellpadding="4" cellspacing="0" class="row1">
         <tr>
            <td align="right">UserID:</td>
            <td><input type="text" name="loginID" size="15"<?php  if ($client->browser == NETSCAPE) { echo("onkeypress=\" if (String.fromCharCode(event.which) == '\\r') { form.passwd.focus(); }\""); } ?> /></td>
	    </tr><tr>
            <td align="right">Password:</td>
            <td><input type="password" name="passwd" size="15"<?php if ($client->browser == NETSCAPE) { echo(" onkeypress=\"if (String.fromCharCode(event.which) == '\\r') { form.submit(); }\""); } ?> /></td>
         </tr>
         <tr class="row0">
            <td colspan="2" align="center"><input name="log_in" type="submit" value="Log In" /></td>
         </tr>
         <tr class="row1">
            <td align="center"><input type="submit" name="new_employer" value="New Employer" /></td>
            <td align="center"><input type="submit" name="forgotten_password" value="Forgot Password" /></td>
         </tr>
      </table>
   </form>
   <br /><div align="left"><p><b class="red">Note:</b> The Microsoft Internet Explorer (MSIE) web browser for Macintosh is no longer supported by <?php echo(BRAND_NAME); ?> 
   because of security concerns. If you are using that browser, please consider using an alternate browser such as Safari or Firefox.</p></div>
   </div><div align="left">
   <p><b class='login'><u>Employers:</u></b> Please enter your assigned <?php echo(BRAND_NAME); ?> UserID and Password and click on the 
   <b>'Log In'</b> button. If you are a new employer without a password, and are interested in posting a job with the 
   <?php echo(ORGANIZATION_CODE); ?> Co-op program, please click on 
	the <b>'New Employer'</b> button above. This will allow you to enter your Contact and Company  
	information to set up your account and submit your job description. If you have forgotten your 
    <?php echo(BRAND_NAME); ?> UserID or Password, please click the <b>'Forgot Password'</b> button above.</p>
   <p><b class='login'><u>Students and Co-op Staff:</u></b> Please log in to the system using your Netlink UserID and Password.</p> 
   <br />
   </div><div align="center">
   <br /><p><b>(Note that you need to have cookies enabled to use this system.)</b></p>
</div>
</td></tr></table>

<?php if (!(($client->browser == NETSCAPE) && ($client->platform == WIN))) { ?>
<script type="text/javascript" language="javascript">
document.loginform.loginID.focus();
</script>
<?php } ?>
