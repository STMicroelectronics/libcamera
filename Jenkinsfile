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
		stage('Build') {
			steps {
				dir('pristine') {
					script {
						sh 'meson setup build --reconfigure --buildtype=release -Dpipelines=rpi/vc4,rpi/pisp -Dipas=rpi/vc4,rpi/pisp -Dv4l2=true -Dgstreamer=enabled -Dtest=false -Dlc-compliance=disabled -Dcam=disabled -Dqcam=disabled -Ddocumentation=disabled -Dpycamera=enabled'
						sh 'ninja -C build'
					}
				}
			}
		}
		stage('Package') {
			when {
				environment name: 'BRANCH_NAME', value: 'debian'
			}
			steps {
				dir('pristine') {
					script {
						sh 'dpkg-buildpackage -us -uc -b'
						sh 'lintian || true'
					}
				}
				script {
					rtUpload (
						serverId: 'artifactory-azure',
						spec: """{ "files": [ {
								"pattern": "*libcamera*.deb",
								"target": "imgswlinux-raspios-local/pool/libcamera/stable/",
								"props": "deb.distribution=stable;deb.component=main;deb.architecture=arm64"
							} ] }"""
					)
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

