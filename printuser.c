#include <pwd.h>
#include <stdio.h>

int main() {

	struct passwd *pas;

	// rewind file
	setpwent();

	// get next entry in password file
	while ((pas = getpwent()) != NULL) {
		printf("user: %s, shell: %s\n", pas->pw_name, pas->pw_shell);
	}

	// close file
	endpwent();

	return(0);
}

