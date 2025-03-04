pipeline {
	environment {
		http_proxy = credentials('proxy')
		https_proxy = credentials('proxy')
	}
	agent {
		docker {
			image 'artifactory.st.com/imgswlinux-docker/raspios_libcamera_pkg_arm64v8:bookworm'
		}
	}
	options {
		skipDefaultCheckout()
	}

	stages {
		stage('Prepare') {
			steps {
				/*
				 * Checkout in a subfolder, as debian packaging
				 * tools require to output on '..'.
				 */
				dir('pristine') {
					checkout scm
				}
			}
		}
		stage('Package') {
			steps {
				dir('pristine') {
					/*
					 * Remove build folder before as Jenkins caches
					 * builds.
					 */
					sh 'rm -rf build/'
					dir('debianizer') {
						git credentialsId: 'af76724f-9593-4f6c-a5cc-1548eb8b0e14',
							url: 'ssh://gitolite@codex.cro.st.com/img-application-sw-linux/debianizer.git'
					}
					script {
						if (env.BRANCH_NAME == 'debian') {
							sh 'debianizer/debianizer.sh'
						} else {
							sh 'debianizer/debianizer.sh --use-origin --snapshot'
						}
					}
				}
			}
		}
		stage('Upload') {
			when { anyOf {
				environment name: 'BRANCH_NAME', value: 'main';
				environment name: 'BRANCH_NAME', value: 'debian'
			}}
			steps {
				script {
					if (env.BRANCH_NAME == 'debian') {
						rtUpload (
							serverId: 'artifactory-azure',
							spec: """{ "files": [ {
									"pattern": "libcamera-ipa*.deb",
									"target": "imgswlinux-raspios-local/pool/libcamera/stable/",
									"props": "deb.distribution=stable;deb.component=main;deb.architecture=arm64"
								} ] }"""
						)
					} else {
						rtUpload (
							serverId: 'artifactory-azure',
							spec: '''{ "files": [ {
									"pattern": "libcamera-ipa*.deb",
									"target": "imgswlinux-raspios-local/pool/libcamera/unstable/",
									"props": "deb.distribution=unstable;deb.component=main;deb.architecture=arm64"
								} ] }'''
						)
					}
				}
			}
		}
	}

	post {
		always {
			// Remove debian packaging stuff
			sh 'find . -maxdepth 1 -type f -name "*libcamera*" -exec rm -v "{}" \\;'
		}
	}
}

