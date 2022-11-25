$(document).ready(function() {

    $('#employer-signin-form').validate({

        rules: {
            email: {
                required: true,
            },
            password: {
                required: true,
            }
        },
        messages: {
            email: {
                required: 'Username is required',
            },
            password: {
                required: 'Password is required',
            },
        }

    });


    $('#employer-sign-up').validate({

        rules: {
            company_name: {
                required: true,
            },
            company_password: {
                required: true,
            },
            confirm_password: {
                required: true,
                equalTo: '#company_password',
            },
            company_email: {
                required: true,
                email: true,
            },
            company_mobile: {
                required: true,
            },
            company_size: {
                required: true,
            },
            company_about: {
                required: true,
            },
            industry_type: {
                required: true,
            },
        },
        messages: {
            company_name: {
                required: 'This field is required',
            },
            company_password: {
                required: 'This field is required',
            },
            confirm_password: {
                required: 'This field is required',
                equalTo: 'Password do not match',
            },
            company_email: {
                required: 'This field is required',
                email: 'Provide a valid email address',
            },
            company_mobile: {
                required: 'This field is required',
            },
            company_size: {
                required: 'This field is required',
            },
            company_about: {
                required: 'This field is required',
            },
            industry_type: {
                required: 'This field is required',
            },
        }

    });

    $('#login-employer').click(function(event) {

        event.preventDefault();

        var that = $('#employer-signin-form'),
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

                if (data == "Incorrect Login Credentials. Username/Password is incorrect") {

                    $('p#employer-login-error').text(data);
                    console.log(data);

                } else if (data == "Your Account has not been activated, Please kindly check your email and activate your account by clicking the provided link") {

                    $('p#employer-login-error').text(data);
                    console.log(data);

                } else if (data == "Sorry, Your Account has been deactivated") {

                    $('p#employer-login-error').text(data);
                    console.log(data);

                } else if (data == "Login Successful") {

                    $('p#employer-login-error').text(data);
                    setTimeout("window.location='employer-dashboard.php'", 1000);
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
