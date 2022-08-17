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
 | Filename: not_in_group.inc                                                   |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/

// filename: not_in_group.inc
// show the not in group screen
?>
</td><td align="left" valign="top"><img src="<?php echo $GLOBALS['colorscheme']['topbar_right']; ?>"></td></tr></table>

     
<table border="0" cellspacing="0" cellpadding="20"><tr><td>

<br />
<br />
<h2 align="center">Warning:  No Co-op Department Assigned to User</h2>
<br />
<h4 align="center" class="bodytext"><?php echo($auth->name); ?></h4>
<br />
<p align="center">You have not been assigned a Co-op group on our computer system.  Please e-mail the following information to your administrator:</a></p>
<ul align="center">
<li>Your name
<li>Your department
<li>Your account login ID
</ul></p>
<br />
<p align="center">Your account may take a few days to process.  If you have interviews that you need to apply to within the next few days, please contact your co-op department to arrange suitable sign-up times.</p>
<br />

</td></tr></table>
