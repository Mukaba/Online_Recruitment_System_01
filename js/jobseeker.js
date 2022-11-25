$(document).ready(function() {

    $('#recovery-form').validate({

      rules: {
          email: {
              required: true,
              email : true,
          }
      },
      messages: {
          email: {
              required: 'Please provide an email address',
              email : 'Please enter a valid email address',
          }
      }

    });

    $('#jobseeker-signin-form').validate({

        rules: {
            username: {
                required: true,
            },
            password: {
                required: true,
            }
        },
        messages: {
            username: {
                required: 'Username is required',
            },
            password: {
                required: 'Password is required',
            }
        }
    });

    $('#login-seeker').click(function(event) {

        event.preventDefault();

        var that = $('#jobseeker-signin-form'),
            url = that.attr('action'),
            type = that.attr('method'),
            data = {};

        // loops through the form to get user information for processing
        that.find('[name]').each(function(index, value) {
            var that = $(this),
                name = that.attr('name'),
                value = that.val();

            data[name] = value;
        });

        $.ajax({
                url: url,
                type: 'POST',
                data: data
            })
            .done(function(data) {

                if (data == "Incorrect Login Credentials. Username/Password is incorrect") {

                    $('p#login-error').css("color", "red");
                    $('p#login-error').text(data);
                    console.log(data);

                } else if (data == "Your Account has not been activated, Please kindly check your email and activate your account by clicking the provided link") {

                    $('p#login-error').css("color", "red");
                    $('p#login-error').text(data);
                    console.log(data);

                } else if (data == "Sorry, Your Account has been deactivated") {

                    $('p#login-error').css("color", "red");
                    $('p#login-error').text(data);
                    console.log(data);

                } else if (data == "Login Successful. You must change your password to conitnue") {

                    $('p#login-error').css("color", "green");
                    $('#login-error').text(data);
                    setTimeout("window.location='changepassword.php'", 2000);
                    console.log(data);

                } else if (data == "Login Successful. You can now update your profile") {

                    $('p#login-error').css("color", "green");
                    $('#login-error').text(data);
                    setTimeout("window.location='proregistration.php'", 2000);
                    console.log(data);

                } else if (data == "Login Successful") {

                    $('p#login-error').css("color", "green");
                    $('#login-error').text(data);
                    setTimeout("window.location='jobseeker-dashboard.php'", 1000);
                    console.log(data);

                } else {

                    console.log(data);

                }

            })

        .fail(function() {
            console.log(data);
        })

    });

    $('#jobseeker-signup-form').validate({

        rules: {
            reg_username: {
                required: true,
            },
            reg_password: {
                required: true,
            },
            reg_confirm_password: {
                required: true,
                equalTo: '#reg_password',
            },
            reg_email: {
                required: true,
                email: true,
            },
            reg_first_name: {
                required: true,
            },
            reg_last_name: {
                required: true,
            }
        },
        messages: {
            reg_username: {
                required: 'This field is required',
            },
            reg_password: {
                required: 'This field is required',
            },
            reg_confirm_password: {
                required: 'This field is required',
                equalTo: 'Passwords do not match',
            },
            reg_email: {
                required: 'This field is required',
                email: "Please provide a correct email address",
            },
            reg_first_name: {
                required: 'This field is required',
            },
            reg_last_name: {
                required: 'This field is required',
            }
        }
    });

    $('#register-jobseeker').click(function(event) {

        event.preventDefault();

        var that = $('#jobseeker-signup-form'),
            url = that.attr('action'),
            type = that.attr('method'),
            data = {};

        // loops through the form to get user information for processing
        that.find('[name]').each(function(index, value) {
            var that = $(this),
                name = that.attr('name'),
                value = that.val();

            data[name] = value;
        });

        $.ajax({
            url: url,
            type: 'POST',
            data: data
        })

        .done(function(data) {

            if (data == "Sorry an account has already been registered with the email: " + $('#reg_email').val()) {

                $('p#email-signup-error').text(data);

                console.log(data);

            } else if (data == "Sorry " + $('#reg_username').val() + " already exist") {

                $('p#username-signup-error').text(data);

                console.log(data);

            } else if (data == "Thanks, " + $('#reg_first_name').val() + " Your registration was succesful. Your activation code has been sent to the email provided. Kindly check your mail") {

                $('p#signup-error').text(data);
                setTimeout("window.location='index.php'", 7000);
                console.log(data);

            } else {

                console.log(data);

            }

        })

        .fail(function() {
            console.log(data);
        })

    });

    $(function() {
        $( "#reg_age" ).datepicker({
          yearRange: "1930:2015",
          changeMonth: true,
          changeYear: true,
          dateFormat: 'dd/mm/yy',
        });
    });

    $('#complete').click(function(event) {

        event.preventDefault();

        var that = $('#pro-registration-form'),
            url = that.attr('action'),
            type = that.attr('method'),
            data = {};

        // loops through the form to get user information for processing
        that.find('[name]').each(function(index, value) {
            var that = $(this),
                name = that.attr('name'),
                value = that.val();

            data[name] = value;
        });

        $.ajax({
                url: url,
                type: 'POST',
                data: data
            })
            .done(function(data) {

                if (data == "Congratulations!!! Your profile has been fully updated.") {

                    $('p#form-notice').css("color", "#fff");
                    $('p#form-notice').text(data);
                    setTimeout("window.location='jobseeker-dashboard.php'", 2000);

                    console.log(data);

                } else if (data == "Ooops!!! Something went wrong. Please try again.") {

                      $('p#form-notice').css("color", "#ff0000");
                      $('p#form-notice').text(data);
                    console.log(data);

                } else {

                    console.log(data);

                }

            })

        .fail(function() {
            console.log(data);
        })

    });
    $('#search-jobs').keyup(function(event) {

        event.preventDefault();

        var that = $('#search-jobs-form'),
            url = that.attr('action'),
            type = that.attr('method'),
            data = {};

        // loops through the form to get user information for processing
        that.find('[name]').each(function(index, value) {
            var that = $(this),
                name = that.attr('name'),
                value = that.val();

            data[name] = value;
        });

        $.ajax({
            url: url,
            type: 'POST',
            data: data,
        })

        .done(function(data) {

            if (data != "false") {

                $('#jobs-table').html(data);

                console.log(data);

            } else if (data != 'true') {

                $('#jobs-table').html(data);

                console.log(data);

            } else if (data == 'string') {

                console.log(data);


            } else {

                console.log(data);

            }

        })

        .fail(function() {
            console.log(data);
        })

    });

});
