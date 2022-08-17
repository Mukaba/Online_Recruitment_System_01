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
 | Filename: email.inc                                                          |
 +------------------------------------------------------------------------------+
 | Description:                                                                 |
 +------------------------------------------------------------------------------+

*/


// filename: email.inc

// usage...
// $mail = new email($address_list, $subject, $message);
// if (!$mail->send()) { echo($mail->error_text); }
//
// $address_list - comma separated
// $mail->cc, $mail->bcc, $mail->from  may also be set
// additional headers may be sent by setting $headers

include_once('misc/functions.inc');


class email
{
	var $to;		// comma separated
	var $subject;
	var $body;
	var $from = BRAND_NAME_SYSTEM_EMAIL;
	var $cc;		// comma separated
	var $bcc;	// comma separated
	var $headers;	// additional headers -- separated by newlines
    var $reply_id;
	// some additional headers... X-Sender:, X-Mailer:, X-Priority:
	
	var $error_text="OK";	// stores a description of any errors
	
	
	
	function email($to=false, $subject=false, $body=false)
	{
		if ($to) { $this->to = trim($to); }
		if ($subject) { $this->subject = $subject; }
		if ($body) { $this->body = $body; }
	}
	
	
	
	function send()
	{
    global $auth;

        $this->to = trim($this->to);
        $this->cc = trim($this->cc);
        $this->bcc = trim($this->bcc);

		if (!$this->to || !isValidEmail($this->to))
		{
			$this->error_text = "Invalid recipient address " . ($this->to ? "(<b>" . $this->to . "</b>)" : "") . ". Please make sure that each e-mail address";
			$this->error_text .= " ends with either a three letter domain code (.com, .net, .org, etc.)";
			$this->error_text .= " or a two letter country code (.ca, .fr, etc.).";
			return false;
		}
		$this->subject = trim($this->subject);
		if (!$this->subject)
		{
			$this->error_text = "No subject specified.";
			return false;
		}
		if (!$this->body)
		{
			$this->error_text = "Message body is empty.";
			return false;
		} else {
            if ($this->reply_id == $auth->contact_id)
            {
                $sql_signature = "SELECT email_signature from contact_internal where contact_id='".$auth->contact_id."'";
                $result_signature = $GLOBALS['dbh']->Execute($sql_signature);
                $row_signature=$result_signature->FetchRow();
                if ($row_signature[0])
                {
                    $this->body.="\n\n";
                    $this->body.="-----------------------------------\n";
                    $this->body.=$row_signature[0];
                } else {
                    if ($contact_info = getContactInfoByID($auth->contact_id)) {
                        $contact = "\n\n";
                        $contact .= "-----------------------------------\n";
                        $contact .= $contact_info["first_name"] . " " . $contact_info["last_name"] . "\n";
                        if ($contact_info["phone"])
                        {
                            $contact .= "Phone: " . $contact_info["phone"] . "\n";
                        }
                        if ($contact_info["fax"])
                        {
                            $contact .= "Fax: " . $contact_info["fax"] . "\n";
                        }
                        if ($contact_info["email"])
                        {
                            $contact .= "E-mail: " . $contact_info["email"] . "\n";
                        }
                        $this->body .= $contact;
                    }
                }
            } else {
                if ($this->reply_id)
                {
                    if ($contact_info = getContactInfoByID($this->reply_id)) {
                        $contact = "\n\n";
                        $contact .= "Please direct responses to...\n";
                        $contact .= "------\n";
                        $contact .= $contact_info["first_name"] . " " . $contact_info["last_name"] . "\n";
                        if ($contact_info["phone"])
                        {
                            $contact .= "Phone: " . $contact_info["phone"] . "\n";
                        }
                        if ($contact_info["fax"])
                        {
                            $contact .= "Fax: " . $contact_info["fax"] . "\n";
                        }
                        if ($contact_info["email"])
                        {
                            $contact .= "E-mail: " . $contact_info["email"] . "\n";
                        }
                        $this->body .= $contact;
                    }
                }
            }
        }
		
		if ($this->cc)
		{
			if (!isValidEmail($this->cc)) 
			{
				$this->error_text = "Invalid CC address list.  Please make sure that each e-mail address"; 
				$this->error_text .= " ends with either a three letter domain code (.com, .net, .org, etc.)";
				$this->error_text .= " or a two letter country code (.ca, .fr, etc.).";
				return false; 
			}
		}
		if ($this->bcc)
		{
			$this->bcc = str_replace("\t"," ",str_replace("\n"," ",$this->bcc));
		        $this->bcc = str_replace(Chr(10)," ",str_replace(Chr(13)," ",$this->bcc));
			if (!isValidEmail($this->bcc)) 
			{ 
				$this->error_text = "Invalid BCC address list.  Please make sure that each e-mail address";
				$this->error_text .= " ends with either a three letter domain code (.com, .net, .org, etc.)";
				$this->error_text .= " or a two letter country code (.ca, .fr, etc.).";
				return false; 
			}
		}
		if (!$this->sendMail($this->to, trim($this->subject), $this->body, $this->headers, $this->from, $this->cc, $this->bcc)) 
		{
			$this->error_text = "SendMail error.";
			return false;
		}
		return true;
	}

	function sendMail($to, $subject, $body, $headers, $from, $cc, $bcc)
	{
		// max number e-mails to send at once (includes to, cc, and bcc)
		$send_max = 50;
		
		$total = 0;
		// replace and extract some things from the strings passed
		$cc = explode (",", $cc);
		$bcc = explode (",", $bcc);
		$to = explode (",", $to);
		$total = sizeof($to) + sizeof($cc) + sizeof($bcc);

		// loop through the number of e-mails to be sent
		for ($t = 0; $t < ceil($total/$send_max); $t ++) {

			// re-initialize
			$arr_to = array ();
			$arr_cc = array ();
			$arr_bcc = array ();
			// reinitialze if needed
			if ($from)
				$send_header = "From: $from\n";
			else
				$send_header = "";

			// now loop though the number of e-mails we can send at once
			for ($i = 0; $i < $send_max; $i ++) {
			
				// this distributes the e-mail back into the correct array
				// do this using push and shift (pull off one, stick on the other)
				// that way the arrays will shrink, then we'll know whether or not to
				// move onto the next array
				if (sizeof($to) > 0 && (is_array ($to)))
					array_push ($arr_to, trim (array_shift ($to)));

				elseif ( (sizeof($to) <= 0) && (sizeof($cc) > 0) && (is_array ($cc)))
					array_push ($arr_cc, trim (array_shift ($cc)));

				elseif ( (sizeof($to) <= 0) && (sizeof($cc) <= 0) && (sizeof($bcc) > 0) && (is_array ($bcc)))
					array_push ($arr_bcc, trim (array_shift ($bcc)));

				if ( (sizeof($to) == 0) && (sizeof($cc) == 0) && (sizeof($bcc) == 0) )
					break;

			} // $i loop 

			// now create the correct string variable 
			if (sizeof ($arr_to) > 0)
				$send_to = trim (implode ($arr_to, ","));
			// this is the garbage e-mail, since we NEED a To: address
			else
				$send_to = NULL_EMAIL;
			if (sizeof ($arr_cc) > 0)
				$send_header .= "cc: ". trim (implode ($arr_cc, ",")) ."\n";
			if (sizeof ($arr_bcc) > 0)
				$send_header .= "bcc: ". trim (implode ($arr_bcc, ",")) ."\n";
			$send_header .= $headers;

			// send it already
			$pipe=popen(PATH_SENDMAIL . " -t -i -f\"$from\" ","w");

			if (!$pipe) return false;
			$message="\n\n$body\n";
			fwrite($pipe, "To: $send_to\n");
			fwrite($pipe,"Errors-To: $from\n");
			fwrite($pipe,"Subject: $subject\n");
			fwrite($pipe,"$send_header\n");
			fwrite($pipe,$message);
			pclose($pipe);

		} // $t loop

		return true;
	}
}

?>
